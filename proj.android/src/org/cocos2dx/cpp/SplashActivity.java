package org.cocos2dx.cpp;

import com.shanonim.necodroid.R;
import org.cocos2dx.lib.Cocos2dxActivity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.Window;

public class SplashActivity extends Cocos2dxActivity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		
		super.onCreate(savedInstanceState);
		setContentView(R.layout.splash);
		
		Handler hdl = new Handler();
		// 500ms遅延させてsplashHandlerを実行します。
		hdl.postDelayed(new splashHandler(), 500);
	}
	
	class splashHandler implements Runnable {
		public void run() {
			Intent intent = new Intent(getApplication(), AppActivity.class);
			startActivity(intent);
			SplashActivity.this.finish();
		}
	}
}
