package android.support.little;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.PixelFormat;
import android.graphics.Point;
import android.graphics.Typeface;
import android.graphics.drawable.GradientDrawable;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.provider.Settings;
import android.util.Log;
import android.view.Display;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.NetworkInterface;
import java.net.URL;
import java.util.Collections;
import java.util.List;

public class MainActivity extends Activity {

	private Point point;

	private WindowManager manager;

	private WindowManager.LayoutParams vParams;

	private View vTouch;

	private WindowManager drawWindow;

	public static int screenWidth, screenHeight;

	private Context mContext;

	static String version = "1.0";

    static String cardCode, deviceCode;

	private String apiInterface;

	private String response;

	private String loginInterface = "http://w5.eydata.net/78457782F5094CE5";

	private String unbindInterface = "http://w5.eydata.net/DC4A6BE6F75C8E70";

	static {
		System.loadLibrary("littleHack");
	}

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
						 WindowManager.LayoutParams.FLAG_FULLSCREEN);
        if (Build.VERSION.SDK_INT >= 19) {
            getWindow().addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
            getWindow().addFlags(WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN);
            getWindow().addFlags(WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS);
            getWindow().addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_NAVIGATION);
		}
		WindowManager window = (WindowManager) getSystemService(Context.WINDOW_SERVICE);
		assert window != null;
        Display display = window.getDefaultDisplay();
        Point screenSize = new Point();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
            display.getRealSize(screenSize);
		}
		if (screenSize.x > screenSize.y) {
            screenWidth = screenSize.x;
            screenHeight = screenSize.y;
        } else {
            screenWidth = screenSize.y;
            screenHeight = screenSize.x;
        }
		GlJniView.ScreenSize(screenWidth, screenHeight);
        loadUI();
		requestPermission();
        if (isVpnUsed()) {
            System.exit(0);
		}
    }

	public void loadUI() {

        this.mContext = this;

        Typeface font = Typeface.createFromAsset(mContext.getAssets(), "TextStyle.ttf");

        GradientDrawable cardRounded = new GradientDrawable();
        cardRounded.setCornerRadius(15);
        cardRounded.setColor(0xFFEBECF0);

        GradientDrawable buttonRounded = new GradientDrawable();
        buttonRounded.setColor(0xFF000000);
        buttonRounded.setCornerRadius(150);
        buttonRounded.setStroke(5, 0xFFFFFFFF);

        GradientDrawable buttonRounded2 = new GradientDrawable();
        buttonRounded2.setColor(0xFF000000);
        buttonRounded2.setCornerRadius(50);

        LinearLayout layout = new LinearLayout(mContext);
        LayoutParams params = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
        layout.setLayoutParams(params);
        layout.setOrientation(LinearLayout.VERTICAL);
        layout.setBackgroundColor(0xFFFFFFFF);
        layout.setGravity(Gravity.CENTER);

        ImageView mainImage = new ImageView(mContext);
        mainImage.setLayoutParams(new LinearLayout.LayoutParams(350, 350));
        try {
            InputStream mInputStream = mContext.getAssets().open("control.png");
            Bitmap bitmap = BitmapFactory.decodeStream(mInputStream);
            mainImage.setImageBitmap(bitmap);
        } catch (IOException e) {}
        layout.addView(mainImage);


        TextView textView = new TextView(mContext);
        textView.setText("Click button to verify card code");
        textView.setTextColor(0xFFB5B5B5);
        textView.setTextSize(14);
        textView.setGravity(Gravity.CENTER);
        textView.setLayoutParams(new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
        LinearLayout.LayoutParams margin1 = (LinearLayout.LayoutParams) textView.getLayoutParams();
        margin1.topMargin = 50;
        textView.setTypeface(font);
        layout.addView(textView);


        final EditText cardCodeInput = new EditText(mContext);
        cardCodeInput.setLayoutParams(new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
        LinearLayout.LayoutParams cardMargin = (LinearLayout.LayoutParams) cardCodeInput.getLayoutParams();
        cardCodeInput.setSingleLine(true);
        cardCodeInput.setTextColor(0xFF777978);
        cardCodeInput.setHint("Enter card code...");
        cardCodeInput.setHintTextColor(0xFF777978);
        cardCodeInput.setText(readFile("/storage/emulated/0/card.log"));
        cardCodeInput.setBackground(cardRounded);
        cardMargin.leftMargin = 50;
        cardMargin.rightMargin = 50;
        cardMargin.topMargin = 50;
        cardCodeInput.setTypeface(font);
        layout.addView(cardCodeInput);

        LinearLayout mainLayout = new LinearLayout(mContext);
        LinearLayout.LayoutParams mainLayoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        mainLayout.setLayoutParams(mainLayoutParams);
        mainLayout.setOrientation(LinearLayout.HORIZONTAL);
        mainLayoutParams.leftMargin = 50;
        mainLayoutParams.rightMargin = 50;
        mainLayoutParams.topMargin = 50;
        layout.addView(mainLayout);

        final Button unbindButton = new Button(mContext);
        LinearLayout.LayoutParams unbindParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        unbindButton.setLayoutParams(unbindParams);
        unbindButton.setText("Unbind");
        unbindButton.setTextColor(0xFFFFFFFF);
        unbindButton.setTextSize(15);
        unbindButton.setTypeface(font);
        unbindButton.setBackground(buttonRounded2);
        unbindParams.weight = 1;
        mainLayout.addView(unbindButton);

        final Button queryButton = new Button(mContext);
        LinearLayout.LayoutParams queryParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        queryButton.setLayoutParams(queryParams);
        queryButton.setText("Query");
        queryButton.setTextColor(0xFFFFFFFF);
        queryButton.setTextSize(15);
        queryButton.setTypeface(font);
        queryButton.setBackground(buttonRounded2);
        queryParams.weight = 1;
        mainLayout.addView(queryButton);


        Button loginButton = new Button(mContext);
        loginButton.setLayoutParams(new LinearLayout.LayoutParams(300, 300));
        LinearLayout.LayoutParams loginMargin = (LinearLayout.LayoutParams) loginButton.getLayoutParams();
        loginButton.setTextColor(0xFFFFFFFF);
        loginButton.setTextSize(15);
        loginButton.setGravity(Gravity.CENTER);
        loginButton.setBackground(buttonRounded);
        loginButton.setText("Device Login");
        loginMargin.topMargin = 50;
        loginButton.setTypeface(font);
        layout.addView(loginButton);


        setContentView(layout);

        loginButton.setOnClickListener(new View.OnClickListener() {

                @Override
                public void onClick(View view) {
                    initializeOverlay();
                    cardCode = cardCodeInput.getText().toString();
                    if (cardCode.length() == 0) {
                        Toast.makeText(mContext, "Please enter card code", Toast.LENGTH_SHORT).show();
                    } else {
                        deviceCode = Settings.System.getString(getContentResolver(), Settings.Secure.ANDROID_ID);
                        apiInterface = "SingleCode=" + cardCode + "&Ver=" + version + "&Mac" + deviceCode;
                        writeFile("/storage/emulated/0/card.log", cardCode);
                        writeFile("/storage/emulated/0/device.log", deviceCode);
                        new Thread(new login()).start();
                    }
                }});

        unbindButton.setOnClickListener(new View.OnClickListener() {

                @Override
                public void onClick(View view) {
                    cardCode = cardCodeInput.getText().toString();
                    if (cardCode.length() == 0) {
                        Toast.makeText(mContext, "Please enter card code", Toast.LENGTH_SHORT).show();
                    } else {
                        deviceCode = Settings.System.getString(getContentResolver(), Settings.Secure.ANDROID_ID);
                        apiInterface = "UserName=" + cardCode + "&UserPwd=&Mac=&Type=";
                        new Thread(new relogin()).start();
                    }
                }});

    }


    class login implements Runnable {

        @Override
        public void run() {
            try {
                URL url = new URL(loginInterface);
                HttpURLConnection httpConnection = (HttpURLConnection) url.openConnection();
                httpConnection.setReadTimeout(9000);
                httpConnection.setRequestMethod("POST");
                OutputStream outputStream = httpConnection.getOutputStream();
                String byteString = "";
                byteString = apiInterface;
                outputStream.write(byteString.getBytes());
                BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(httpConnection.getInputStream()));
                String string = "";
                StringBuffer stringBuffer = new StringBuffer();
                while ((string = bufferedReader.readLine()) != null) {
                    stringBuffer.append(string);
                }
                response = stringBuffer.toString();
                Message msg = new Message();
                msg.what = 1;
                handler.sendMessage(msg);
            } catch (IOException e) {}

        }
    }

    Handler handler = new Handler() {

        @Override
        public void handleMessage(Message msg) {

            super.handleMessage(msg);

            switch (msg.what) {
                case 1:
                    if (response.equals("-401")) {
                        Toast.makeText(MainActivity.this, "Card code error", Toast.LENGTH_SHORT).show();
                    } else if (response.equals("-402")) {
                        Toast.makeText(MainActivity.this, "Device code error", Toast.LENGTH_SHORT).show();
                    } else if (response.equals("-406")) {
                        Toast.makeText(MainActivity.this, "Card code not found", Toast.LENGTH_SHORT).show();
                    } else if (response.equals("-410")) {
                        Toast.makeText(MainActivity.this, "Card code expired", Toast.LENGTH_SHORT).show();
                    } else if (response.length() == 32) {
                        Toast.makeText(MainActivity.this, "Login successful", Toast.LENGTH_LONG).show();
                        initializeOverlay();
                        Intent intent = new Intent();
                        intent.setClassName("com.tencent.tmgp.pubgmhd", "com.epicgames.ue4.SplashActivity");
                        startActivity(intent);
                        finish();
                    }
            }
        }
    };

    class relogin implements Runnable {

        @Override
        public void run() {
            try {
                URL url = new URL(unbindInterface);
                HttpURLConnection httpConnection = (HttpURLConnection) url.openConnection();
                httpConnection.setReadTimeout(9000);
                httpConnection.setRequestMethod("POST");
                OutputStream outputStream = httpConnection.getOutputStream();
                String byteString = "";
                byteString = apiInterface;
                outputStream.write(byteString.getBytes());
                BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(httpConnection.getInputStream()));
                String string = "";
                StringBuffer stringBuffer = new StringBuffer();
                while ((string = bufferedReader.readLine()) != null) {
                    stringBuffer.append(string);
                }
                response = stringBuffer.toString();
                Message msg = new Message();
                msg.what = 2;
                handler2.sendMessage(msg);
            } catch (IOException e) {}
        }
    }

    Handler handler2 = new Handler() {

        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);

            switch (msg.what) {
                case 2:
                    if (response.equals("-21")) {
                        Toast.makeText(MainActivity.this, "Unbind successful", Toast.LENGTH_SHORT).show();
                    } else if (response.equals("-503")) {
                        Toast.makeText(MainActivity.this, "Unbind will expire", Toast.LENGTH_SHORT).show();
                    } else {
                        Toast.makeText(MainActivity.this, "Unbind error", Toast.LENGTH_SHORT).show();
                    }
            }
        }
    };


    public static boolean isVpnUsed() {
        try {
            List<NetworkInterface> nis = Collections.list(NetworkInterface.getNetworkInterfaces());
            if (nis != null) {
                for (NetworkInterface intf : nis) {
                    if (!intf.isUp() || intf.getInterfaceAddresses().size() == 0) {
                        continue;
                    }
                    if ("tun0".equals(intf.getName()) || "ppp0".equals(intf.getName())) {
                        return true;
                    }
                }
            }
        } catch (Throwable e) {
            e.printStackTrace();
        }
        return false;
    }


	public void initializeOverlay() {
		try {
			InputStream in = getAssets().open("TextStyle.ttf");
			GlJniView.fontData = new byte[in.available()];
			in.read(GlJniView.fontData);
		} catch (Exception e) {}
		drawWindow = (WindowManager) getSystemService(Context.WINDOW_SERVICE);
		manager = (WindowManager) getApplicationContext().getSystemService(Context.WINDOW_SERVICE);
		vParams = getAttributes(false);
        final WindowManager.LayoutParams wParams = getAttributes(true);

        GlJniView display = new GlJniView(this);
		vTouch = new View(this);
        manager.addView(vTouch, vParams);
        manager.addView(display, wParams);

		vTouch.setOnTouchListener(new View.OnTouchListener() {
				@Override
				public boolean onTouch(View v, MotionEvent event) {
					int action = event.getAction();
					switch (action) {
						case MotionEvent.ACTION_MOVE:
						case MotionEvent.ACTION_DOWN:
						case MotionEvent.ACTION_UP:
							GlJniView.MotionEventClick(action != MotionEvent.ACTION_UP, event.getRawX(), event.getRawY());
							break;
						default:
							break;
					}
					return false;
				}
			});
        final Handler updateHandler = new Handler();
        updateHandler.postDelayed(new Runnable() {
				@Override
				public void run() {
					try {
						String rect[] = GlJniView.getWindowRect().split("\\|");
						vParams.x = Integer.parseInt(rect[0]);
						vParams.y = Integer.parseInt(rect[1]);
						vParams.width = Integer.parseInt(rect[2]);
						vParams.height = Integer.parseInt(rect[3]);
						manager.updateViewLayout(vTouch, vParams);
					} catch (Exception e) {
					}
					updateHandler.postDelayed(this, 20);
				}
			}, 20);
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
		point = new Point();
        drawWindow.getDefaultDisplay().getRealSize(point);
		GlJniView.real(point.x, point.y);
		Log.e("NDK", "Screen rotated");
    }

	private WindowManager.LayoutParams getAttributes(boolean isWindow) {
        WindowManager.LayoutParams params = new WindowManager.LayoutParams();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            params.type = WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY;
        } else {
            params.type = WindowManager.LayoutParams.TYPE_PHONE;
        }
        params.flags = WindowManager.LayoutParams.FLAG_FULLSCREEN |
			WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS |
			WindowManager.LayoutParams.FLAG_TRANSLUCENT_NAVIGATION |
			WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;
        if (isWindow) {
            params.flags |= WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL | WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE;
        }
        params.format = PixelFormat.RGBA_8888;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            params.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
        }
        params.gravity = Gravity.LEFT | Gravity.TOP;
        params.x = params.y = 0;
        params.width = params.height = isWindow ? WindowManager.LayoutParams.MATCH_PARENT : 0;
        return params;
    }

	public void requestPermission() {

        boolean isGranted = true;

        if (android.os.Build.VERSION.SDK_INT >= 23) {
            if (this.checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                isGranted = false;
            }
            if (this.checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                isGranted = false;
            }
            if (this.checkSelfPermission(Manifest.permission.RECORD_AUDIO) != PackageManager.PERMISSION_GRANTED) {
                isGranted = false;
            }

            if (!isGranted) {
                this.requestPermissions(
                    new String[]{
                        Manifest.permission.ACCESS_COARSE_LOCATION,
                        Manifest.permission.ACCESS_FINE_LOCATION,
                        Manifest.permission.READ_EXTERNAL_STORAGE,
                        Manifest.permission.RECORD_AUDIO,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE
                    },
                    102);
            }
        }
        if (Build.VERSION.SDK_INT >= 23) {
            if (!Settings.canDrawOverlays(MainActivity.this)) {
                Intent intent = new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION,
                                           Uri.parse("package:" + getPackageName()));
                startActivityForResult(intent, 111);
            }
        }
    }


	public static void createFile(String path) {
        File mFile = new File(path);
        if (mFile.exists()) {
            mFile.delete();
        }
        try {
            mFile.createNewFile();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

	public static boolean fileExists(String strFile) {
        try {
            File f = new File(strFile);
            if (!f.exists()) {
	            return false;
            }
        } catch (Exception e) {
            return false;
        }
        return true;
    }

	public static boolean writeFile(String path, String txt) {
        byte[] sourceByte = txt.getBytes();
        if (null != sourceByte) {
            try {
                File file = new File(path);
                if (!file.exists()) {
                    File dir = new File(file.getParent());
                    dir.mkdirs();
                    file.createNewFile();
                }
                FileOutputStream outStream = new FileOutputStream(file);
                outStream.write(sourceByte);
                outStream.close();
                return true;
            } catch (Exception e) {
                e.printStackTrace();
                return false;
            }
        }
        return true;
    }

	public static String readFile(String path) {
        String str = "";
        try {
            File urlFile = new File(path);
            InputStreamReader isr = new InputStreamReader(new FileInputStream(urlFile), "UTF-8");
            BufferedReader br = new BufferedReader(isr);

            String line = null;
            while ((line = br.readLine()) != null) {
                str = str + line;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return str;
    }

	public static boolean exportAssetsFile(Context context, String outPath, String fileName) {
        File file = new File(outPath);
        if (!file.exists()) {
            if (!file.mkdirs()) {
                Log.e("--Method--", "copyAssetsSingleFile: cannot create directory.");
                return false;
            }
        }
        try {
            InputStream inputStream = context.getAssets().open(fileName);
            File outFile = new File(file, fileName);
            FileOutputStream fileOutputStream = new FileOutputStream(outFile);
            byte[] buffer = new byte[1024];
            int byteRead;
            while (-1 != (byteRead = inputStream.read(buffer))) {
                fileOutputStream.write(buffer, 0, byteRead);
            }
            inputStream.close();
            fileOutputStream.flush();
            fileOutputStream.close();
            return true;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
	}
}
