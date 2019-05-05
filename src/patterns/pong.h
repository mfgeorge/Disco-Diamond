// Pong Game Pattern
#include "pattern.h"
#include "colors.h"

#define DEBUG 0

class PongPattern : public Pattern {
   public:
      PongPattern(uint16_t ball_width, uint16_t wall_width, 
         uint16_t wall_spacing, uint16_t anchor, 
         int16_t range_start, int16_t range_end) : 
         ball_width_(ball_width), wall_width_(wall_width), 
         wall_spacing_(wall_spacing), anchor_(anchor), 
         range_start_(range_start), range_end_(range_end) {
            if (ball_width_ == 0){
               // ball_width can't be less than 1
               ball_width_ = 1;
            }
            ball_pos_ = 0;
            // step for the first time to set up the boundaries
            step();
         }

      void init(PatternFrame* pattern_frame) override {
         uint8_t num_dims = pattern_frame->activeDimensions;
         // draw this pattern on the highest layer of the current 
         // dimension during init. make new dimension if none exist
         PatternDimension *dimension;
         if (num_dims == 0) {
            dimension = pattern_frame->AddDimension();
         }
         else {
            uint8_t last_dim = num_dims - 1;
            dimension = &(pattern_frame->dimensions[last_dim]);
         }
         range_ = dimension->AddLayer();
         *range_ = LinearRange(range_start_, range_end_, 
            [this](uint16_t curr, uint16_t total) {
               return PongGenerator(curr, total);
            });
#if DEBUG
         Serial.println("Pong Pattern Initialized!");
#endif
      } 

      void step() override {
         // ball_pos_ is the center of the ball
         ball_pos_ = growing_ ? (ball_pos_ + 1) : (ball_pos_ - 1);

         wall_one_lower_ = anchor_;
         wall_one_upper_ = wall_one_lower_ + wall_width_;

         wall_two_lower_ = wall_one_upper_ + wall_spacing_;
         wall_two_upper_ = wall_two_lower_ + wall_width_;

         ball_lower_ = wall_one_lower_ + 1 + ball_pos_ - ball_width_/2;
         ball_upper_ = wall_one_lower_ + 1 + ball_pos_ + ball_width_/2;
         if (ball_width_%2){
            ball_upper_ = ball_upper_ + 1;
         }
#if DEBUG
         Serial.println(" ");
         Serial.println(" ");
         Serial.printf("wall 1 lower: %u..", wall_one_lower_);
         Serial.printf("wall 1 upper: %u..", wall_one_upper_); 
         Serial.printf("wall 2 lower: %u..", wall_two_lower_);
         Serial.printf("wall 2 upper: %u..", wall_two_upper_);
         Serial.printf("ball_lower: %u..", ball_lower_);
         Serial.printf("ball upper: %u", ball_upper_);
         Serial.println(" ");
#endif 
         if (ball_upper_ >= wall_two_lower_){
            growing_ = false;
            NextColor(); 
         }
         else if (ball_lower_ <= wall_one_upper_){
            growing_ = true;
            NextColor();
         }

      }

      uint16_t frame_delay() {
         uint16_t BPM = 64;
         // every two beats should hit the walls
         return 1000*60/BPM/2/wall_spacing_;
      }

      Pixel PongGenerator(uint16_t curr, uint16_t total) {

         if ((curr >= ball_lower_) && (curr < ball_upper_)) {
            return ball_color_;
         }
         else if ((curr >= wall_one_lower_) && (curr < wall_one_upper_)){
            return wall_color_;
         }
         else if ((curr >= wall_two_lower_) && (curr < wall_two_upper_)){
            return wall_color_;
         }
         else {
            return background_color_;
         }
         
      }

      void NextColor(){
         wall_color_ = ball_color_;
         color_index_ = (color_index_+1)%2;

         if (color_index_ == 0){
            ball_color_ = BLUE;
         }
         else if (color_index_ == 1) {
            ball_color_ = RED; 
         }
         else if (color_index_ == 2) {
            ball_color_ = GREEN;
         }
         else if (color_index_ == 3) {
            ball_color_ = WHITE;
         }

      }

   private:
      uint16_t ball_width_;
      uint16_t wall_width_;
      uint16_t wall_spacing_;
      uint16_t anchor_;
      int16_t range_start_;
      int16_t range_end_;
      uint16_t ball_pos_;
      bool growing_=true;
      LinearRange* range_;

      Pixel ball_color_ = BLUE;
      Pixel wall_color_ = WHITE;
      Pixel background_color_ = BLACK;

      uint16_t wall_one_lower_; 
      uint16_t wall_one_upper_;
      uint16_t wall_two_lower_;
      uint16_t wall_two_upper_;
      uint16_t ball_lower_;
      uint16_t ball_upper_;
      uint8_t color_index_; 
};