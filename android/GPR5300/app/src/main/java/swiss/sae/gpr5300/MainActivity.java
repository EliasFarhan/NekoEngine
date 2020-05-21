package swiss.sae.gpr5300;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import org.libsdl.app.SDLActivity;

public class MainActivity extends SDLActivity {
    private static native void load(AssetManager mgr);
    private static native void destroy(String directory);
    private AssetManager mgr;

    @SuppressLint("SourceLockedOrientationActivity")
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
        mgr = getResources().getAssets();
        load(mgr);
    }


    @Override
    protected void onDestroy() {
        String directory  = getFilesDir().toString();
        destroy(directory);
        super.onDestroy();
    }

}
