package swiss.sae.gpr5300;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;

import org.libsdl.app.SDLActivity;

import java.io.File;

public class MainActivity extends SDLActivity {
    private static final int WRITE_REQUEST_CODE = 142;

    private static native void load(AssetManager mgr);
    private static native void finalize(String directory);
    private AssetManager mgr;

    @SuppressLint("SourceLockedOrientationActivity")
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED)
            {
                requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, WRITE_REQUEST_CODE);
            }
            else
            {
                File externalDirectory = getExternalFilesDir("easy_profile");
                Log.println(Log.DEBUG, "NekoEngine","Manage to get external directory");
                String path = externalDirectory.getAbsolutePath();
                externalDirectory.mkdir();
                Log.println(Log.DEBUG, "NekoEngine","Profile directory: "+path);
            }
        }
        mgr = getResources().getAssets();
        load(mgr);
    }

    @Override
    protected void onStop() {
        super.onStop();
        Log.println(Log.DEBUG, "NekoEngine","On Finalize");
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M)
        {
            if (checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED)
            {
                Finalize();
            }

        }
        else
        {
            Finalize();
        }

    }

    protected void Finalize()
    {
        Log.println(Log.DEBUG, "NekoEngine","Permission is good!");
        try
        {
            File externalDirectory = getExternalFilesDir("easy_profile");
            Log.println(Log.DEBUG, "NekoEngine","Manage to get external directory");
            String path = externalDirectory.getAbsolutePath();
            externalDirectory.mkdir();
            Log.println(Log.DEBUG, "NekoEngine","Profile directory: "+path);

            finalize(path);
        }
        catch (Exception e)
        {
            Log.println(Log.ERROR, "NekoEngine", "Exception for external dir... with message: \n"+e.getMessage());
        }

        Log.println(Log.DEBUG, "NekoEngine","Profile finished successfully");
    }

}
