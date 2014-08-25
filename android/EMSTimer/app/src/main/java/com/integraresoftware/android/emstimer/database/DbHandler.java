package com.integraresoftware.android.emstimer.database;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;


public class DbHandler extends SQLiteOpenHelper {

	public static final int DATABASE_VERSION = 1;
	public static final String DATABASE_NAME = "TimesRecorder.db";

	private static final String TEXT_TYPE = " TEXT, ";
	private static final String INT_TYPE = " INT, ";


	public DbHandler(Context context) {
		super(context, DATABASE_NAME, null, DATABASE_VERSION);
	}

	@Override
	public void onCreate(SQLiteDatabase db) {
		String SQL_CREATE_TIMES_TABLE =
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

		db.execSQL(SQL_CREATE_TIMES_TABLE);
	}

	@Override
	public void onUpgrade(SQLiteDatabase sqLiteDatabase, int i, int i2) {
		//TODO have to work on this one if I want to change it
	}


}
