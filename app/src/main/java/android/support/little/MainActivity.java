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
import android.support.little.GlJniView;
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

	private WindowManager 绘制窗口;

	public static int 屏幕宽,屏幕高;

	private Context mContext;

	static String 版本号="1.0";

    static String 卡密,机器码;

	private String 接口;

	private String 返回;

	private String 登录接口="http://w5.eydata.net/78457782F5094CE5";

	private String 解绑接口="http://w5.eydata.net/DC4A6BE6F75C8E70";

	static{
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
		WindowManager 窗口=(WindowManager)getSystemService(Context.WINDOW_SERVICE);
		assert 窗口 != null;
        Display 分辨率=窗口.getDefaultDisplay();
        Point 输出分辨=new Point();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
            分辨率.getRealSize(输出分辨);
		}
		if (输出分辨.x > 输出分辨.y) {
            屏幕宽 = 输出分辨.x;
            屏幕高 = 输出分辨.y;
        } else {
            屏幕宽 = 输出分辨.y;
            屏幕高 = 输出分辨.x;
        }
		GlJniView.ScreenSize(屏幕宽, 屏幕高);
        加载页面();
		requestPermission();
        if (isVpnUsed())
        {
            System.exit(0);   
		}
    }

	public void 加载页面() {

        this.mContext = this;

        Typeface 字体 = Typeface.createFromAsset(mContext.getAssets(), "TextStyle.ttf");//放到as里面

        GradientDrawable 卡密圆角 = new GradientDrawable();
        卡密圆角.setCornerRadius(15);
        卡密圆角.setColor(0xFFEBECF0);
        //卡密圆角.setStroke(2, 0xFFFFFFFF);

        GradientDrawable 按钮圆角 = new GradientDrawable();
        按钮圆角.setColor(0xFF000000);
        按钮圆角.setCornerRadius(150);
        按钮圆角.setStroke(5, 0xFFFFFFFF);

        GradientDrawable 按钮圆角2 = new GradientDrawable();
        按钮圆角2.setColor(0xFF000000);
        按钮圆角2.setCornerRadius(50);

        LinearLayout layout = new LinearLayout(mContext);
        LayoutParams params = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
        layout.setLayoutParams(params);
        layout.setOrientation(LinearLayout.VERTICAL);
        layout.setBackgroundColor(0xFFFFFFFF);
        layout.setGravity(Gravity.CENTER);

        ImageView 主图 = new ImageView(mContext);
        主图.setLayoutParams(new LinearLayout.LayoutParams(350, 350));
        try {
            InputStream mInputStream = mContext.getAssets().open("control.png");
            Bitmap bitmap = BitmapFactory.decodeStream(mInputStream);
            主图.setImageBitmap(bitmap);
        } catch (IOException e) {}
        layout.addView(主图);


        TextView 文本 = new TextView(mContext);
        文本.setText("点击按钮验证卡密");
        文本.setTextColor(0xFFB5B5B5);
        文本.setTextSize(14);
        文本.setGravity(Gravity.CENTER);
        文本.setLayoutParams(new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
        LinearLayout.LayoutParams 间距1= (LinearLayout.LayoutParams) 文本.getLayoutParams();
        间距1.topMargin = 50;
        文本.setTypeface(字体);//设置字体
        layout.addView(文本);


        final EditText 卡密框 = new EditText(mContext);
        卡密框.setLayoutParams(new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
        LinearLayout.LayoutParams 卡密间距= (LinearLayout.LayoutParams) 卡密框.getLayoutParams();
        卡密框.setSingleLine(true);
        卡密框.setTextColor(0xFF777978);
        卡密框.setHint("请输入卡密......");
        卡密框.setHintTextColor(0xFF777978);
        卡密框.setText(读取文件("/storage/emulated/0/卡密.log"));
        卡密框.setBackground(卡密圆角);
        卡密间距.leftMargin = 50;
        卡密间距.rightMargin = 50;
        卡密间距.topMargin = 50;
        卡密框.setTypeface(字体);//设置字体
        layout.addView(卡密框);

        LinearLayout 主布局 = new LinearLayout(mContext);
        LinearLayout.LayoutParams 主布局Params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        主布局.setLayoutParams(主布局Params);
        主布局.setOrientation(LinearLayout.HORIZONTAL);
        主布局Params.leftMargin = 50;
        主布局Params.rightMargin = 50;
        主布局Params.topMargin = 50;
        layout.addView(主布局);

        final Button 解绑按钮 = new Button(mContext);
        LinearLayout.LayoutParams 解绑按钮Params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        解绑按钮.setLayoutParams(解绑按钮Params);
        解绑按钮.setText("解绑");
        解绑按钮.setTextColor(0xFFFFFFFF);
        解绑按钮.setTextSize(15);
        解绑按钮.setTypeface(字体);
        解绑按钮.setBackground(按钮圆角2);
        解绑按钮Params.weight = 1;
        主布局.addView(解绑按钮);

        final Button 查卡按钮 = new Button(mContext);
        LinearLayout.LayoutParams 查卡按钮Params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        查卡按钮.setLayoutParams(查卡按钮Params);
        查卡按钮.setText("查卡");
        查卡按钮.setTextColor(0xFFFFFFFF);
        查卡按钮.setTextSize(15);
        查卡按钮.setTypeface(字体);
        查卡按钮.setBackground(按钮圆角2);
        查卡按钮Params.weight = 1;
        主布局.addView(查卡按钮);


        Button 登录按钮 = new Button(mContext);
        登录按钮.setLayoutParams(new LinearLayout.LayoutParams(300, 300));
        LinearLayout.LayoutParams 登录间距= (LinearLayout.LayoutParams) 登录按钮.getLayoutParams();
        登录按钮.setTextColor(0xFFFFFFFF);
        登录按钮.setTextSize(15);
        登录按钮.setGravity(Gravity.CENTER);
        登录按钮.setBackground(按钮圆角);
        登录按钮.setText("设备登录");
        登录间距.topMargin = 50;
        登录按钮.setTypeface(字体);//设置字体
        layout.addView(登录按钮);


        setContentView(layout);

        登录按钮.setOnClickListener(new View.OnClickListener() {

                @Override 
                public void onClick(View view) {     
                    xhnry();
                    卡密 = 卡密框.getText().toString();
                    if (卡密.length() == 0) {
                        Toast.makeText(mContext, "请输入卡密", Toast.LENGTH_SHORT).show();
                    } else {
                        机器码 = Settings.System.getString(getContentResolver(), Settings.Secure.ANDROID_ID);
                        接口 = "SingleCode=" + 卡密 + "&Ver=" + 版本号 + "&Mac" + 机器码;
                        写入文件("/storage/emulated/0/卡密.log", 卡密);
                        写入文件("/storage/emulated/0/设备码.log", 机器码);
                        new Thread(new login()){}.start();
                    }
                }});

        解绑按钮.setOnClickListener(new View.OnClickListener() {

                @Override 
                public void onClick(View view) {
                    卡密 = 卡密框.getText().toString();
                    if (卡密.length() == 0) {
                        Toast.makeText(mContext, "请输入卡密", Toast.LENGTH_SHORT).show();
                    } else {
                        机器码 = Settings.System.getString(getContentResolver(), Settings.Secure.ANDROID_ID);
                        接口 = "UserName=" + 卡密 + "&UserPwd=&Mac=&Type=";
                        new Thread(new relogin()){}.start();
                    }
                }});

    }


    
    class login implements Runnable {

        @Override
        public void run() {
            try {
                URL url=new URL(登录接口);
                HttpURLConnection HttpURLConnection=(HttpURLConnection) url.openConnection();
                HttpURLConnection.setReadTimeout(9000);
                HttpURLConnection.setRequestMethod("POST");
                OutputStream outputStream = HttpURLConnection.getOutputStream();
                String byteString="";
                byteString = 接口;
                outputStream.write(byteString.getBytes());
                BufferedReader BufferedReader=new BufferedReader(new InputStreamReader(HttpURLConnection.getInputStream()));
                String String="";
                StringBuffer StringBuffer=new StringBuffer();
                while ((String = BufferedReader.readLine()) != null) {
                    StringBuffer.append(String);
                }
                返回 = StringBuffer.toString();
                Message ee=new Message();
                ee.what = 1;
                hd.sendMessage(ee);
            } catch (IOException e) {}

        }
    }

    Handler hd=new Handler(){

        @Override
        public void handleMessage(Message msg) {

            super.handleMessage(msg);

            switch (msg.what) {
                case 1:
                    if (返回.equals("-401")) {
                        Toast.makeText(MainActivity.this, "卡密错误", Toast.LENGTH_SHORT).show();
                    } else if (返回.equals("-402")) {

                        Toast.makeText(MainActivity.this, "机器码错误", Toast.LENGTH_SHORT).show();
                    } else if (返回.equals("-406")) {

                        Toast.makeText(MainActivity.this, "卡密不存在", Toast.LENGTH_SHORT).show();
                    } else if (返回.equals("-410")) {

                        Toast.makeText(MainActivity.this, "卡密已过期", Toast.LENGTH_SHORT).show();
                    } else if (返回.length() == 32) {

                        Toast.makeText(MainActivity.this, "登录成功", Toast.LENGTH_LONG).show();
                        xhnry();                                         
                        Intent intent = new Intent();
                        intent.setClassName("com.tencent.tmgp.pubgmhd", "com.epicgames.ue4.SplashActivity");
                        startActivity(intent);
                        finish();

                    }
            }
        };
    };

    class relogin implements Runnable {

        @Override
        public void run() {
            try {
                URL url=new URL(解绑接口);
                HttpURLConnection HttpURLConnection=(HttpURLConnection) url.openConnection();
                HttpURLConnection.setReadTimeout(9000);
                HttpURLConnection.setRequestMethod("POST");
                OutputStream outputStream = HttpURLConnection.getOutputStream();
                String byteString="";
                byteString = 接口;
                outputStream.write(byteString.getBytes());
                BufferedReader BufferedReader=new BufferedReader(new InputStreamReader(HttpURLConnection.getInputStream()));
                String String="";
                StringBuffer StringBuffer=new StringBuffer();
                while ((String = BufferedReader.readLine()) != null) {
                    StringBuffer.append(String);
                }
                返回 = StringBuffer.toString();
                Message ee=new Message();
                ee.what = 2;
                hd2.sendMessage(ee);
            } catch (IOException e) {}

        }
    }

    //处理
    Handler hd2=new Handler(){

        @Override
        public void handleMessage(Message msg) {

            super.handleMessage(msg);

            switch (msg.what) {
                case 2:
                    if (返回.equals("-21")) {
                        Toast.makeText(MainActivity.this, "解绑成功", Toast.LENGTH_SHORT).show();
                    } else if (返回.equals("-503")) {
                        Toast.makeText(MainActivity.this, "解绑后将过期", Toast.LENGTH_SHORT).show();
                    } else 
                        Toast.makeText(MainActivity.this, "解绑错误", Toast.LENGTH_SHORT).show();
            }
        };
	};


    public static boolean isVpnUsed()
    {//vpn检测
        try
        {
            List<NetworkInterface> nis = Collections.list(NetworkInterface.getNetworkInterfaces());
            if (nis != null)
            {
                for (NetworkInterface intf : nis)
                {
                    if (!intf.isUp() || intf.getInterfaceAddresses().size() == 0)
                    {
                        continue;
                    }
                    if ("tun0".equals(intf.getName()) || "ppp0".equals(intf.getName()))
                    {
                        return true; // The VPN is up
                    }
                }
            }
        }
        catch (Throwable e)
        {
            e.printStackTrace();
        }
        return false;
    }
	

	public void xhnry() {
		try {
			InputStream in =getAssets().open("TextStyle.ttf");
			GlJniView.fontData = new byte[in.available()];
			in.read(GlJniView.fontData);		
		} catch (Exception e) {}
		绘制窗口 = (WindowManager)getSystemService(Context.WINDOW_SERVICE);
		manager = (WindowManager) getApplicationContext().getSystemService(Context.WINDOW_SERVICE);
		vParams = getAttributes(false);
        final WindowManager.LayoutParams wParams = getAttributes(true);

        GlJniView display = new GlJniView(this);
		vTouch = new View(this);
        manager.addView(vTouch, vParams);
        manager.addView(display, wParams);

		//获取悬浮窗触摸
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
        final Handler handler = new Handler();
        handler.postDelayed(new Runnable() {
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
					handler.postDelayed(this, 20);
				}
			}, 20);
	}
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
		point = new Point();
        绘制窗口.getDefaultDisplay().getRealSize(point);
		GlJniView.real(point.x, point.y);
		Log.e("NDK", "屏幕旋转了");
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
        params.format = PixelFormat.RGBA_8888;            // 设置图片格式，效果为背景透明
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            params.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
        }
        params.gravity = Gravity.LEFT | Gravity.TOP;        // 调整悬浮窗显示的停靠位置为左侧置顶
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
            if (this.checkSelfPermission(Manifest.permission.RESTART_PACKAGES) != PackageManager.PERMISSION_GRANTED) {
                isGranted = false;
            }
            if (this.checkSelfPermission(Manifest.permission.REQUEST_COMPANION_RUN_IN_BACKGROUND) != PackageManager.PERMISSION_GRANTED) {
                isGranted = false;
            }
            if (this.checkSelfPermission(Manifest.permission.REQUEST_IGNORE_BATTERY_OPTIMIZATIONS) != PackageManager.PERMISSION_GRANTED) {
                isGranted = false;
            }
            if (this.checkSelfPermission(Manifest.permission.SYSTEM_ALERT_WINDOW) != PackageManager.PERMISSION_GRANTED) {
                isGranted = false;
            }

            if (!isGranted) {
                this.requestPermissions(
                    new String[]{Manifest.permission.ACCESS_COARSE_LOCATION, Manifest.permission
                        .ACCESS_FINE_LOCATION,
                        Manifest.permission.READ_EXTERNAL_STORAGE,
                        Manifest.permission.RECORD_AUDIO,
                        Manifest.permission.RESTART_PACKAGES,
                        Manifest.permission.REQUEST_COMPANION_RUN_IN_BACKGROUND,
                        Manifest.permission.REQUEST_IGNORE_BATTERY_OPTIMIZATIONS,
                        Manifest.permission.SYSTEM_ALERT_WINDOW,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE},
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


	public static void 创建文件(String path) {
        //新建一个File类型的成员变量，传入文件名路径。
        File mFile = new File(path);
        //判断文件是否存在，存在就删除
        if (mFile.exists()) {
            mFile.delete();
        }
        try {
            //创建文件
            mFile.createNewFile();
            //给一个吐司提示，显示创建成功

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

	public static boolean 文件是否存在(String strFile) {
        try {
            File f=new File(strFile);
            if (!f.exists()) {
	            return false;
            }
        } catch (Exception e) {
            return false;
        }
        return true;
    }

	/*
     * 写入文件内容
     */
    public static boolean 写入文件(String path, String txt) {
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

    /*
     * 读取文件内容
     */
    public static String 读取文件(String path) {
        String str = "";
        try {
            File urlFile = new File(path);
            InputStreamReader isr = new InputStreamReader(new FileInputStream(urlFile), "UTF-8");
            BufferedReader br = new BufferedReader(isr);

            String mimeTypeLine = null;
            while ((mimeTypeLine = br.readLine()) != null) {
                str = str + mimeTypeLine;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return str;
    }

	public static boolean 写出assets资源文件(Context context, String outPath, String fileName) {
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
            // Transfer bytes from inputStream to fileOutputStream
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
