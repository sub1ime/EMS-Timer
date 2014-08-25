package com.integraresoftware.android.emstimer.database;


import android.content.ContentProvider;
import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.UriMatcher;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteQueryBuilder;
import android.net.Uri;

public class DbProvider extends ContentProvider {
	// for debugging
	public static final String TAG = "DbProvider";

	// content provider and authority
	public static final String AUTHORITY = "com.integraresoftware.android.emstimer.database.DbProvider";
	public static final Uri TIMES_CONTENT_URI = Uri.parse("content://" + AUTHORITY + "/times");

	// MIME types for searching
	public static final String TIMES_MIME_TYPE = ContentResolver.CURSOR_DIR_BASE_TYPE
			+ "/vnd.com.integraresoftware.android.emstimer.database.time";
	public static final String TIME_MIME_TYPE = ContentResolver.CURSOR_ITEM_BASE_TYPE
			+ "/vnd.com.integraresoftware.android.emstimer.database.time";

	// URI matcher
	private static final int LIST_TIMES = 1;
	private static final int ITEM_TIME = 2;
	private static final UriMatcher sUriMatcher = buildMatcher();

	// get database
	private SQLiteDatabase mDb;

	@Override
	public boolean onCreate() {
		mDb = new DbHandler(getContext()).getReadableDatabase();
		return true;
	}

	private static UriMatcher buildMatcher() {
		UriMatcher matcher = new UriMatcher(UriMatcher.NO_MATCH);

		matcher.addURI(AUTHORITY, "time", LIST_TIMES);
		matcher.addURI(AUTHORITY, "time/#", ITEM_TIME);

		return matcher;
	}

	@Override
	public String getType(Uri uri) {
		switch(sUriMatcher.match(uri)) {
			case LIST_TIMES:
				return TIMES_MIME_TYPE;
			case ITEM_TIME:
				return TIME_MIME_TYPE;
			default:
				throw new IllegalArgumentException("Unknown URI: " + uri);
		}
	}

	@Override
	public Cursor query(Uri uri, String[] projection, String selection, String[] selectionArgs, String sortOrder) {
		// create the query builder
		SQLiteQueryBuilder queryBuilder = new SQLiteQueryBuilder();
		// sett the tables the query builder will look into
		queryBuilder.setTables(TimesContract.TABLE_NAME);

		// determine if it wants a full list or just one
		int uriType = sUriMatcher.match(uri);
		switch (uriType) {
			// wants the full list
			case LIST_TIMES:
				break;
			// wants a specific time
			case ITEM_TIME:
				// add the id of the time to get
				queryBuilder.appendWhere(TimesContract.COLUMN_ID + "=" + uri.getLastPathSegment());
				break;
			default:
				throw new IllegalArgumentException("Unknown Uri: " + uri);
		}

		Cursor cursor = queryBuilder.query(mDb, projection, selection, selectionArgs, null, null, sortOrder);
		cursor.setNotificationUri(getContext().getContentResolver(), uri);

		return cursor;
	}

	@Override
	public Uri insert(Uri uri, ContentValues contentValues) {
		return null;
	}

	@Override
	public int delete(Uri uri, String s, String[] strings) {
		return 0;
	}

	@Override
	public int update(Uri uri, ContentValues contentValues, String s, String[] strings) {
		return 0;
	}
}
