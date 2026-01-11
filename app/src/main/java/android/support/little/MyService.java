package android.support.little;

import android.annotation.SuppressLint;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.graphics.PixelFormat;
import android.graphics.Point;
import android.os.Build;
import android.os.IBinder;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import java.io.InputStream;

public class MyService extends Service {

	@SuppressLint("StaticFieldLeak")
    private static MyService Instance;
    private WindowManager manager;
    private GlJniView drawView;
	private Point point;
	public static int screenWidth, screenHeight;

    @Override
    public void onCreate() {
        super.onCreate();
		// Get real resolution - compatible with Android <= 11
		manager = (WindowManager) this.getSystemService(Context.WINDOW_SERVICE);
		point = new Point();
		manager.getDefaultDisplay().getRealSize(point);
		screenWidth = point.x;
		screenHeight = point.y;
		if (screenWidth < screenHeight) {
			screenWidth = point.y;
			screenHeight = point.x;
		}
		try {
			InputStream in = getAssets().open("zt.ttf");
			GlJniView.fontData = new byte[in.available()];
			in.read(GlJniView.fontData);
		} catch (Exception e) {}
		// Create float window view
        SetFloatView();
    }

    public void SetFloatView() {

		drawView = new GlJniView(this);

        manager = (WindowManager) this.getSystemService(Context.WINDOW_SERVICE);
        WindowManager.LayoutParams wmParams = new WindowManager.LayoutParams();
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            wmParams.type = WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY;
        } else {
            wmParams.type = WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY;
        }
        wmParams.format = PixelFormat.RGBA_8888;
		wmParams.flags = WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL | WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE;
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            wmParams.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
        }
		wmParams.systemUiVisibility = 5894;
        wmParams.gravity = Gravity.LEFT | Gravity.TOP;
		wmParams.x = wmParams.y = 0;
        wmParams.width = wmParams.height = false ? WindowManager.LayoutParams.MATCH_PARENT : 0;
        manager.addView(drawView, wmParams);

		drawView.setOnTouchListener(new View.OnTouchListener() {
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
					return true;
				}
			});
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if (drawView != null) {
            manager.removeView(drawView);
        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

}
