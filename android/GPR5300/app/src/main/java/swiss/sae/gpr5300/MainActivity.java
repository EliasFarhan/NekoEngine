package swiss.sae.gpr5300;

import android.content.Intent;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.app.Activity;
import android.view.View;

import org.libsdl.app.SDLActivity;

public class MainActivity extends SDLActivity {
    private static native void load(AssetManager mgr);

    private AssetManager mgr;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mgr = getResources().getAssets();
        load(mgr);
    }
}
