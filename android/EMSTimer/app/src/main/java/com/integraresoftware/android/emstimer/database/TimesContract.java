package com.integraresoftware.android.emstimer.database;

import android.provider.BaseColumns;

public class TimesContract implements BaseColumns {

	// empty constructor to prevent someone from accidentially instantiating the
	// contract class
	public TimesContract() { };

	public static final String TABLE_NAME = "times";
	public static final String COLUMN_ID = "_id";
	public static final String COLUMN_DATE = "date";
	public static final String COLUMN_ENROUTE = "en_route";
	public static final String COLUMN_ONSCENE = "on_scene";
	public static final String COLUMN_CONTACT = "contact";
	public static final String COLUMN_TRANSPORT = "transport";
	public static final String COLUMN_HOSPITAL = "hospital";
	public static final String COLUMN_CLEAR = "clear";
	public static final String COLUMN_VALID = "valid";
	
}
