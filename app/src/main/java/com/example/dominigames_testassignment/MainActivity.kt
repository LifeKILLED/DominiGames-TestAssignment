package com.example.dominigames_testassignment

import android.content.Context
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.util.Log
import android.view.MotionEvent
import androidx.appcompat.app.AppCompatActivity
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MainActivity : AppCompatActivity() {

    companion object {
        init {
            System.loadLibrary("dominigames_testassignment")
        }
    }

    private lateinit var glView: GLSurfaceView

    // --- JNI functions ---
    external fun initLoader(assetManager: android.content.res.AssetManager)
    external fun stringFromJNI(): String

    external fun onPointerStart(id: Int, x: Float, y: Float)
    external fun onPointerMove(id: Int, x: Float, y: Float)
    external fun onPointerEnd(id: Int)

    private external fun SceneLoadFromFile(filename: String)
    private external fun SceneUpdate()
    private external fun SceneDraw()
    private external fun RendererBeginFrame()
    private external fun RendererEndFrame()
    private external fun RendererSetViewport(width: Int, height: Int)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        initLoader(assets)

        val hello = stringFromJNI()
        Log.i("NativeGL", hello)

        glView = TouchGLSurfaceView(this)
        setContentView(glView)
    }

    override fun onResume() {
        super.onResume()
        glView.onResume()
    }

    override fun onPause() {
        glView.onPause()
        super.onPause()
    }

    inner class TouchGLSurfaceView(context: Context) : GLSurfaceView(context) {

        init {
            setEGLContextClientVersion(2) // OpenGL ES 2.0
            setRenderer(RendererImpl())
            renderMode = RENDERMODE_CONTINUOUSLY
        }

        override fun onTouchEvent(event: MotionEvent): Boolean {
            val pointerIndex = event.actionIndex
            val pointerId = event.getPointerId(pointerIndex)

            when (event.actionMasked) {
                MotionEvent.ACTION_DOWN,
                MotionEvent.ACTION_POINTER_DOWN -> {
                    onPointerStart(pointerId, event.getX(pointerIndex), event.getY(pointerIndex))
                }

                MotionEvent.ACTION_MOVE -> {
                    for (i in 0 until event.pointerCount) {
                        val id = event.getPointerId(i)
                        onPointerMove(id, event.getX(i), event.getY(i))
                    }
                }

                MotionEvent.ACTION_UP,
                MotionEvent.ACTION_POINTER_UP,
                MotionEvent.ACTION_CANCEL -> {
                    onPointerEnd(pointerId)
                }
            }

            return true
        }
    }

    inner class RendererImpl : GLSurfaceView.Renderer {
        override fun onSurfaceCreated(glUnused: GL10?, config: EGLConfig?) {
            SceneLoadFromFile("cube_scene.ini")
        }

        override fun onSurfaceChanged(glUnused: GL10?, width: Int, height: Int) {
            RendererSetViewport(width, height)
        }

        override fun onDrawFrame(glUnused: GL10?) {
            RendererBeginFrame()
            SceneUpdate()
            SceneDraw()
            RendererEndFrame()
        }
    }
}
