package android.support.little;

import android.content.Context;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class GlJniView extends SurfaceView implements SurfaceHolder.Callback {

    // --- Восстанавливаем поле fontData (убирает ошибку в MyService) ---
    public static byte[] fontData; 

    private Thread renderThread;
    private boolean isRunning = false;

    public GlJniView(Context context) {
        super(context);
        getHolder().addCallback(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        // Инициализация Vulkan
        initVulkan(holder.getSurface());
        
        isRunning = true;
        // Исправлено: Используем new Runnable() вместо лямбды (убирает синтаксические ошибки)
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

    // --- Native Methods (Новые) ---
    public static native void initVulkan(Surface surface);
    public static native void resize(int width, int height);
    public static native void step();
    public static native void imgui_Shutdown();
    public static native void MotionEventClick(boolean down, float PosX, float PosY);

    // --- Методы совместимости (Убирают ошибки в MainActivity) ---
    
    // MainActivity вызывает это. Просто перенаправляем вызов в resize.
    public static void ScreenSize(int width, int height) {
        resize(width, height);
    }

    // MainActivity вызывает это при повороте. Тоже перенаправляем.
    public static void real(int width, int height) {
        resize(width, height);
    }
    
    // MainActivity вызывает getWindowRect. Возвращаем заглушку.
    // Если вам нужна реальная логика, её нужно реализовать, но для компиляции этого достаточно.
    public static String getWindowRect() {
        return "0|0|1080|2400"; 
    }
}