#include <pebble.h>

// the only window
static Window *window;

// layers
static TextLayer *time_text_layer;
static TextLayer *text_layer;
static Layer *seperator_layer;
static TextLayer *status_text_layer;
static ActionBarLayer *action_bar;
static TextLayer *status_text_layer_adult;
// six status layers
static TextLayer *status_text_layer1;
static TextLayer *status_text_layer2;
static TextLayer *status_text_layer3;
static TextLayer *status_text_layer4;
static TextLayer *status_text_layer5;
static TextLayer *status_text_layer6;

// needed variables
bool started;
int elapsed_seconds;
char time_display[] = "0:00:00";
int progress;
const int idle = 0;
const int enRoute = 1;
const int onScene = 2;
const int ptContact = 3;
const int transporting = 4;
const int hospital = 5;
const int clear = 6;
int status_origin_y;
int status_origin_x;
char current_time[] = "00:00";
char enroute_time[] = "00:00";
char onscene_time[] = "00:00";
char contact_time[] = "00:00";
char transport_time[] = "00:00";
char hospital_time[] = "00:00";
char clear_time[] = "00:00";

// bitmaps
static GBitmap *action_icon_play;
static GBitmap *action_icon_next;
static GBitmap *action_icon_reset;

// callback for each second passed
void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	if (started) {
		elapsed_seconds++;
		// stop the clock if over 10 hours has passed
		if (elapsed_seconds == 3600){
			started = false;
			// do something to let the user know the app will not handle 
			// a call longer then 10 hours
		}
			
		// string-ify elapsed time and add leading zeros if needed 
		// if less then 10 minutes and less then 10 seconds
		if ((elapsed_seconds % 60) < 10 && elapsed_seconds < 600 ) {
			snprintf(
				time_display, 
				sizeof(time_display), 
				"%d:0%d:0%d", 
				(elapsed_seconds / 3600), 
				(elapsed_seconds / 60), 
				(elapsed_seconds % 60));
			// if greater then or equal to 10 minutes but less then 10 seconds
		} else if (elapsed_seconds < 600) {
			snprintf(
				time_display,
				sizeof(time_display),
				"%d:0%d:%d",
				(elapsed_seconds / 3600),
				(elapsed_seconds / 60),
				(elapsed_seconds % 60));
			// if less then 10 seconds, but greater then 10 minutes
		} else if ((elapsed_seconds % 60) < 10 && elapsed_seconds > 599) {
			snprintf(
				time_display, 
				sizeof(time_display), 
				"%d:%d:0%d", 
				(elapsed_seconds / 3600), 
				(elapsed_seconds / 60), 
				(elapsed_seconds % 60));
			// for greater then 10 minutes and greater then 10 seconds
		} else {
			snprintf(
				time_display,
				sizeof(time_display),
				"%d:%d:%d",
				(elapsed_seconds / 3600),
				(elapsed_seconds / 60),
				(elapsed_seconds % 60));
		}
		
		text_layer_set_text(time_text_layer, time_display);
	}
}

static void build_seperator_layer(Layer *layer, GContext *ctx) {
	GRect bounds = layer_get_bounds(layer);
	
	// a black filled rectangle
	graphics_context_set_fill_color(ctx, GColorBlack);
	graphics_fill_rect(ctx, bounds, 0, GCornerNone);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	
	if (progress == idle) {
		progress++;
		started = true;
		layer_set_hidden((Layer *) status_text_layer, true);
		clock_copy_time_string(current_time, sizeof(current_time));
		strncpy(enroute_time, current_time, 5);
		text_layer_set_text(status_text_layer1, enroute_time);
		// change images
		action_bar_layer_set_icon(action_bar, BUTTON_ID_SELECT, action_icon_next);
		action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, action_icon_reset);
	} else if (progress == enRoute) {
		progress++;
		clock_copy_time_string(current_time, sizeof(current_time));
		strncpy(onscene_time, current_time, 5);
		text_layer_set_text(status_text_layer2, onscene_time);
	} else if (progress == onScene) {
		progress++;
		clock_copy_time_string(current_time, sizeof(current_time));
		strncpy(contact_time, current_time, 5);
		text_layer_set_text(status_text_layer3, contact_time);
	} else if (progress == ptContact) {
		progress++;
		clock_copy_time_string(current_time, sizeof(current_time));
		strncpy(hospital_time, current_time, 5);
		text_layer_set_text(status_text_layer4, hospital_time);
	} else if (progress == transporting) {
		progress++;
		clock_copy_time_string(current_time, sizeof(current_time));
		strncpy(clear_time, current_time, 5);
		text_layer_set_text(status_text_layer5, clear_time);
	} else if (progress == hospital) {
		progress++;
		started = false;
		clock_copy_time_string(current_time, sizeof(current_time));
		text_layer_set_text(status_text_layer6, current_time);
		action_bar_layer_set_icon(action_bar, BUTTON_ID_SELECT, action_icon_play);
		action_bar_layer_clear_icon(action_bar, BUTTON_ID_UP);
	} else {
		progress = idle;
		elapsed_seconds = 0;
		text_layer_set_text(status_text_layer1, "--:--");
		text_layer_set_text(status_text_layer2, "--:--");
		text_layer_set_text(status_text_layer3, "--:--");
		text_layer_set_text(status_text_layer4, "--:--");
		text_layer_set_text(status_text_layer5, "--:--");
		text_layer_set_text(status_text_layer6, "--:--");
		snprintf(
				time_display, 
				sizeof(time_display), 
				"%d:0%d:0%d", 
				(elapsed_seconds / 3600), 
				(elapsed_seconds / 60), 
				(elapsed_seconds % 60));
		text_layer_set_text(time_text_layer, time_display);
		
	}
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
	if (started) {
		started = false;
		clock_copy_time_string(current_time, sizeof(current_time));
		text_layer_set_text(status_text_layer6, current_time);
		action_bar_layer_clear_icon(action_bar, BUTTON_ID_UP);
		action_bar_layer_set_icon(action_bar, BUTTON_ID_SELECT, action_icon_play);
		progress = clear;
	}
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
}

static void window_load(Window *window) {
	action_bar = action_bar_layer_create();
	action_bar_layer_add_to_window(action_bar, window);
	action_bar_layer_set_click_config_provider(action_bar, click_config_provider);

	action_bar_layer_set_icon(action_bar, BUTTON_ID_SELECT, action_icon_play);

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  const int16_t width = bounds.size.w - ACTION_BAR_WIDTH - 3;

	// create "time elaped" later
  text_layer = text_layer_create((GRect) { .origin = {0, 0 }, .size = { width, 20} });
  text_layer_set_text(text_layer, "Time Elapsed:");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
	// create layer to house the timer
  time_text_layer = text_layer_create((GRect) { .origin = { 0,20 }, .size = { width, 30 } });
  text_layer_set_font(time_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text(time_text_layer, time_display);
  text_layer_set_text_alignment(time_text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(time_text_layer));
	// create a boundry layer
	seperator_layer = layer_create(GRect(0, 51, width - 2, 3));
	layer_set_update_proc(seperator_layer, build_seperator_layer);
	layer_add_child(window_layer, seperator_layer);
	
  
  status_origin_y = 54;
  status_origin_x = 70;
  int width_short = 30;
  int seperator = 14;
  // create six layers for the display of the times
  status_text_layer1 = text_layer_create((GRect) { .origin = { status_origin_x,status_origin_y }, .size = { width_short, 15 } });
  text_layer_set_text(status_text_layer1, "--:--");
  layer_add_child(window_layer, text_layer_get_layer(status_text_layer1));
  
  status_origin_y += seperator;
  status_text_layer2 = text_layer_create((GRect) { .origin = { status_origin_x,status_origin_y }, .size = { width_short, bounds.size.h - 20 } });
  text_layer_set_text(status_text_layer2, "--:--");
  layer_add_child(window_layer, text_layer_get_layer(status_text_layer2));
  
  status_origin_y += seperator;
  status_text_layer3 = text_layer_create((GRect) { .origin = { status_origin_x,status_origin_y }, .size = { width_short, bounds.size.h - 20 } });
  text_layer_set_text(status_text_layer3, "--:--");
  layer_add_child(window_layer, text_layer_get_layer(status_text_layer3));
  
  status_origin_y += seperator;
  status_text_layer4 = text_layer_create((GRect) { .origin = { status_origin_x,status_origin_y }, .size = { width_short, bounds.size.h - 20 } });
  text_layer_set_text(status_text_layer4, "--:--");
  layer_add_child(window_layer, text_layer_get_layer(status_text_layer4));

  status_origin_y += seperator;
  status_text_layer5 = text_layer_create((GRect) { .origin = { status_origin_x,status_origin_y }, .size = { width_short, bounds.size.h - 20 } });
  text_layer_set_text(status_text_layer5, "--:--");
  layer_add_child(window_layer, text_layer_get_layer(status_text_layer5));

  status_origin_y += seperator;
  status_text_layer6 = text_layer_create((GRect) { .origin = { status_origin_x,status_origin_y }, .size = { width_short, bounds.size.h - 20 } });
  text_layer_set_text(status_text_layer6, "--:--");
  layer_add_child(window_layer, text_layer_get_layer(status_text_layer6));

  
  status_text_layer_adult = text_layer_create((GRect) { .origin = { 0, 54 }, .size = { 68, bounds.size.h - 53 } });
  text_layer_set_text(status_text_layer_adult, "En Route: \nArrived: \nContact: \nTransport: \nHospital: \nCleared: ");
  layer_add_child(window_layer, text_layer_get_layer(status_text_layer_adult));

// create a status layer
  status_text_layer = text_layer_create((GRect) { .origin = { 0,54 }, .size = { width, bounds.size.h - 50 } });
  text_layer_set_text(status_text_layer, "Press play to mark en route.");
  layer_add_child(window_layer, text_layer_get_layer(status_text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
  text_layer_destroy(time_text_layer);
  layer_destroy(seperator_layer);
  text_layer_destroy(status_text_layer);

	// six status layers
	text_layer_destroy(status_text_layer1);
	text_layer_destroy(status_text_layer2);
	text_layer_destroy(status_text_layer3);
	text_layer_destroy(status_text_layer4);
	text_layer_destroy(status_text_layer5);
	text_layer_destroy(status_text_layer6);
	text_layer_destroy(status_text_layer_adult);
	
  action_bar_layer_destroy(action_bar);
}

static void init(void) {
	// create action bar
  action_icon_play = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ACTION_ICON_PLAY);
  action_icon_next = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ACTION_ICON_NEXT);
  action_icon_reset = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ACTION_ICON_RESET);

	// create window
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
 
	// initialize stopwatch
	elapsed_seconds = 0;
	started = false;
	tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
 
	// initialize state
	progress = idle;
	
	// push window
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
    window_destroy(window);
	// desstroy images
    gbitmap_destroy(action_icon_play);
	gbitmap_destroy(action_icon_next);
	gbitmap_destroy(action_icon_reset);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
