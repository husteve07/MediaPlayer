package com.example.mediaplayer;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.example.mediaplayer.databinding.ActivityMainBinding;

public class OpenPath extends AppCompatActivity
{
    private Button btfile;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.openpath);
        btfile = findViewById(R.id.playvideo);
        btfile.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view)
            {
                EditText t = findViewById(R.id.fileurl);
                Open(t.getText().toString());


                finish();
            }
        });

    }
    public native void Open(String url);
}
