package com.integraresoftware.android.emstimer.database;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;


public class DbHandler extends SQLiteOpenHelper {

	public static final int DATABASE_VERSION = 1;
	public static final String DATABASE_NAME = "TimesRecorder.db";

	private static final String TEXT_TYPE = " TEXT, ";
	private static final String INT_TYPE = " INT, ";

	private static final String SQL_CREATE_TIMES_TABLE =
			"CREATE TABLE " + TimesContract.TABLE_NAME
					+ " (" + TimesContract.COLUMN_ID + "INTEGER PRIMARY KEY,"
					+ TimesContract.COLUMN_DATE + TEXT_TYPE
					+ TimesContract.COLUMN_ENROUTE + TEXT_TYPE
					+ TimesContract.COLUMN_ONSCENE + TEXT_TYPE
					+ TimesContract.COLUMN_CONTACT + TEXT_TYPE
					+ TimesContract.COLUMN_TRANSPORT + TEXT_TYPE
					+ TimesContract.COLUMN_HOSPITAL + TEXT_TYPE
					+ TimesContract.COLUMN_CLEAR + TEXT_TYPE
					+ TimesContract.COLUMN_VALID + INT_TYPE;


	public DbHandler(Context context) {
		super(context, DATABASE_NAME, null, DATABASE_VERSION);
	}

	@Override
	public void onCreate(SQLiteDatabase db) {
		db.execSQL(SQL_CREATE_TIMES_TABLE);
	}

	@Override
	public void onUpgrade(SQLiteDatabase db, int i, int i2) {
		// Drop the table and delete all data
		db.execSQL("DROP TABLE IF EXISTS " + TimesContract.TABLE_NAME);

		onCreate(db);
	}


}
