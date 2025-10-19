package com.example.dominigames_testassignment

import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {

    companion object {
        init {
            System.loadLibrary("dominigames_testassignment")
        }
    }

    external fun initLoader(assetManager: android.content.res.AssetManager)

    external fun stringFromJNI(): String

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        initLoader(assets)

        val hello = stringFromJNI()
        Log.i("NativeGL", hello)
    }
}
