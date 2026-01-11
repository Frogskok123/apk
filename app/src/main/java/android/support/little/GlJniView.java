package android.support.little;

import android.content.Context;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class GlJniView extends SurfaceView implements SurfaceHolder.Callback {

    // Font data for ImGui rendering
    public static byte[] fontData;

    private Thread renderThread;
    private boolean isRunning = false;

    public GlJniView(Context context) {
        super(context);
        getHolder().addCallback(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        // Initialize Vulkan
        initVulkan(holder.getSurface());
        
        isRunning = true;
        // Use anonymous Runnable class for Java 8 compatibility
        renderThread = new Thread(new Runnable() {
            @Override
            public void run() {
                while (isRunning) {
                    step();
                }
            }
        });
        renderThread.start();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        resize(width, height);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        isRunning = false;
        try {
            renderThread.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        imgui_Shutdown();
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        int action = event.getAction();
        boolean down = (action == MotionEvent.ACTION_DOWN || action == MotionEvent.ACTION_MOVE);
        MotionEventClick(down, event.getX(), event.getY());
        return true;
    }

    // Native methods
    public static native void initVulkan(Surface surface);
    public static native void resize(int width, int height);
    public static native void step();
    public static native void imgui_Shutdown();
    public static native void MotionEventClick(boolean down, float posX, float posY);

    /**
     * Get screen configuration and orientation through JNI
     * Returns string in format: "width|height|orientation"
     * orientation: 0 = Portrait, 1 = Landscape
     */
    public static native String getScreenConfig();

    // Compatibility methods - forward calls to resize
    public static void ScreenSize(int width, int height) {
        resize(width, height);
    }

    public static void real(int width, int height) {
        resize(width, height);
    }
    
    public static String getWindowRect() {
        return "0|0|1080|2400";
    }
}
