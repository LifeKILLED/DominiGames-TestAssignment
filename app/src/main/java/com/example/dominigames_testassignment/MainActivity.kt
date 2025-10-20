package com.example.dominigames_testassignment

import android.os.Bundle
import android.util.Log
import android.view.MotionEvent
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {

    companion object {
        init {
            System.loadLibrary("dominigames_testassignment")
        }
    }

    external fun initLoader(assetManager: android.content.res.AssetManager)
    external fun stringFromJNI(): String

    external fun onPointerStart(id: Int, x: Float, y: Float)
    external fun onPointerMove(id: Int, x: Float, y: Float)
    external fun onPointerEnd(id: Int)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        initLoader(assets)

        val hello = stringFromJNI()
        Log.i("NativeGL", hello)
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
