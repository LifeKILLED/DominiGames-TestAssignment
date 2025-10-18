package com.example.dominigames_testassignment

class MainActivity {
    companion object {
        init {
            System.loadLibrary("dominigames_testassignment")
        }
    }

    external fun stringFromJNI(): String
}
