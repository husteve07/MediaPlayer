package com.example.mediaplayer;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;

import com.example.mediaplayer.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity{

    // Used to load the 'mediaplayer' library on application startup.
    static {
        System.loadLibrary("mediaplayer");
    }

    private Button button;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);


        supportRequestWindowFeature(Window.FEATURE_NO_TITLE);

        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN
        );

        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

        setContentView(R.layout.activity_main);

        button = findViewById(R.id.open_button);

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.e("MediaPlayer", "open button clicked");
                Intent intent = new Intent();
                intent.setClass(MainActivity.this, OpenPath.class);
                startActivity(intent);
            }

        });
    }



}