package com.heb12.android;

import android.app.Activity;
import android.os.Bundle;
import android.view.Window;
import android.webkit.WebSettings;
import android.widget.TextView;
import android.webkit.WebView;
import android.webkit.JavascriptInterface;
import android.text.TextUtils;
import android.support.v7.app.AppCompatActivity;

public class MainActivity extends Activity
{
    @Override
    public void onCreate(Bundle savedInstanceState) {
    	requestWindowFeature(Window.FEATURE_NO_TITLE);
        super.onCreate(savedInstanceState);

        WebView webview = new WebView(this);
		webview.setWebContentsDebuggingEnabled(true);
        WebSettings settings = webview.getSettings();
		settings.setJavaScriptEnabled(true);

		settings.setBuiltInZoomControls(true);
		settings.setDomStorageEnabled(true);
		settings.setLoadWithOverviewMode(true);
		webview.loadUrl("file:///android_asset/index.html");

		webview.addJavascriptInterface(new backend(), "b");

        setContentView(webview);
    }

	public class backend {
		@JavascriptInterface
		public String getVerses(String reference) {
			return TextUtils.join("<br>", arrayTest(reference));
		}
	}

	public native String[] arrayTest(String ref);

	static {
        System.loadLibrary("backend");
    }
}
