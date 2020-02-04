package com.example.googlehome;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.CompoundButton;
import android.widget.ImageView;
import android.widget.Switch;
import android.widget.Toast;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends AppCompatActivity {
    ImageView fanImg;
    ImageView bulbImg;
    String lightStatus;
    String fanStatus;
    Switch bulbSwitch;
    Switch fanSwitch;
    final FirebaseDatabase database = FirebaseDatabase.getInstance();
    final DatabaseReference myRef = database.getReference("on");
    final DatabaseReference myRefFan = database.getReference("fanon");

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initialize();  // initializing all views
        action();  // actions to perform
    }

    private void initialize() {
        bulbImg = findViewById(R.id.imageViewBulb);
        fanImg = findViewById(R.id.imageViewFan);
        fanSwitch = findViewById(R.id.switchFan);
        bulbSwitch = findViewById(R.id.switchBulb);
    }

    private void action() {
        fanSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if (b) {
                    myRefFan.setValue("1");
                    fanImg.setImageResource(R.drawable.fanon);
                } else {
                    myRefFan.setValue("0");
                    fanImg.setImageResource(R.drawable.fan_icon);
                }
            }
        });
        bulbSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if (b) {
                    myRef.setValue("1");
                    bulbImg.setImageResource(R.drawable.bulb_icon);
                } else {
                    myRef.setValue("2");
                    bulbImg.setImageResource(R.drawable.bulb_icon_off);
                }
            }
        });

        // Read from the database
        myRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                // This method is called once with the initial value and again
                // whenever data at this location is updated.
                lightStatus = dataSnapshot.getValue(String.class);
                Toast.makeText(getApplicationContext(), "Value is " + lightStatus, Toast.LENGTH_LONG).show();
                Log.d("msg", "Value is: " + lightStatus);
            }

            @Override
            public void onCancelled(DatabaseError error) {
                // Failed to read value
                Log.w("msg", "Failed to read value.", error.toException());
            }
        });


        // Read from the database
        myRefFan.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                // This method is called once with the initial value and again
                // whenever data at this location is updated.
                fanStatus = dataSnapshot.getValue(String.class);
                Toast.makeText(getApplicationContext(), "Value is " + fanStatus, Toast.LENGTH_LONG).show();
                Log.d("msg", "Value is: " + fanStatus);
            }

            @Override
            public void onCancelled(DatabaseError error) {
                // Failed to read value
                Log.w("msg", "Failed to read value.", error.toException());
            }
        });

    }

//    private void checkLightStatus() {
//        if (lightStatus == "1") {
//            // Write a message to the database
////            myRef.setValue("1");
//            bulbImg.setImageResource(R.drawable.bulb_icon);
//        } else {
//            // Write a message to the database
////            myRef.setValue("2");
//            bulbImg.setImageResource(R.drawable.bulb_icon_off);
//        }
//    }
//
//    private void checkFanStatus() {
//        if (fanStatus == "1") {
//            // Write a message to the database
////            myRefFan.setValue("1");
//            fanImg.setImageResource(R.drawable.fanon);
//        } else {
//            // Write a message to the database
////            myRefFan.setValue("0");
//            fanImg.setImageResource(R.drawable.fan_icon);
//        }
//    }

}
