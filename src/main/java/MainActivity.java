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

public class MainActivity extends Activity {
	static {System.loadLibrary("backend");}
	public native String cloadTranslation(String indexLocation, String textLocation);
	public native String[] cgetVerses(String ref);

	@Override
	public void onCreate(Bundle savedInstanceState) {
		// Don't want the ugly top bar
		requestWindowFeature(Window.FEATURE_NO_TITLE);

		super.onCreate(savedInstanceState);

		WebView webview = new WebView(this);
		webview.setWebContentsDebuggingEnabled(true);

		WebSettings settings = webview.getSettings();
		settings.setJavaScriptEnabled(true);
		settings.setBuiltInZoomControls(true);
		settings.setDomStorageEnabled(true);
		settings.setLoadWithOverviewMode(true);

		webview.loadUrl("file:///android_asset/ui/output.html");
		webview.addJavascriptInterface(new backend(), "b");

		setContentView(webview);

		// Load web translation
	}

	public class backend {
		@JavascriptInterface
		public String getVerses(String reference) {
			try {
				return TextUtils.join("\n", cgetVerses(reference));
			} catch (Exception e) {
				return "It broke";
			}
		}

		@JavascriptInterface
		public String loadTranslation(String indexLocation, String textLocation) {
			return cloadTranslation(indexLocation, textLocation);
		}
	}
}
