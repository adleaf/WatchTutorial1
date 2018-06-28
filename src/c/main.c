#include <pebble.h>



static Window *s_main_window;

//this has been added
static BitmapLayer *RPI_image_layer;
//this has been added
static GBitmap *RPI_image;
//this has been added
Window *window;

static TextLayer *s_time_layer;

static void update_time(){
  //get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  //write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
  
  //display this time on the textlayer
  text_layer_set_text(s_time_layer, s_buffer);
  
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  update_time();
}

static void main_window_load(Window *window){
  
  //get informatio about the window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  //this has been added
  RPI_image_layer = bitmap_layer_create(GRect(-1, 0, 180, 180));
  //this has been added
 // RPI_image = gbitmap_create_with_resource(RESOURCE_ID_RPI);
    RPI_image = gbitmap_create_with_resource(RESOURCE_ID_RPI);

  //the next 4 have been added
  bitmap_layer_set_compositing_mode(RPI_image_layer, GCompOpAssign);
  bitmap_layer_set_bitmap(RPI_image_layer, RPI_image);
  bitmap_layer_set_alignment(RPI_image_layer, GAlignCenter);
  layer_add_child(window_layer, bitmap_layer_get_layer(RPI_image_layer));
  
  //create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  
  //Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  //text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  //add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
}

static void main_window_unload(Window *window){
  //destroy text layer
  text_layer_destroy(s_time_layer);
  
  //this has been added
  gbitmap_destroy(RPI_image);
  //this has been added
  bitmap_layer_destroy(RPI_image_layer);
}

static void init(){
  //create main window element and assign it to a pointer
  s_main_window = window_create();
  
  //set handlers to manage the elements inside the window
  window_set_window_handlers(s_main_window, (WindowHandlers){
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  //this has been added
 // const bool animated = true;
  //this has been added
 // window_stack_push(window, animated);
  
  //Show the window on this watch with animated = true
  window_stack_push(s_main_window, true);
  
  update_time();
  
  //register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler); 
}

static void deinit(){
  
  //destroy window
  window_destroy(s_main_window);
}




int main(void){
  init();
  app_event_loop();
  deinit();
}



