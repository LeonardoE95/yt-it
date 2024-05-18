// Based on COMP 4300 Game Programming
// 
// Assignment I
// https://www.youtube.com/watch?v=UARdwTLUH5U&list=PL_xRyXins848nDj2v-TJYahzvs-XW9sVV&index=4
// 
// Author: Leonardo Tamiano
//
// Compilation: make all

#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <cstring>
#include <format>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

// -------------------------------------------------------------------

const std::string WINDOW_TITLE = "COMP4300";
const std::string CONFIG_FILE = "./bin/config.txt";
const char *IMGUI_TITLE = "COMP4300 - Assignment I";

sf::Font FONT;

// ---------------------------------------------------------------

// Window 720 720
struct WindowConfig {
  int width;
  int height;
};

// Font bin/fonts/symbola-hint.ttf 18 255 255 255
struct FontConfig {
  std::string path;
  int size;
  int R, G, B;
};

struct UserInput {
  int window_width;
  int window_height;
  
  bool draw;
  float scale;
  float velX;
  float velY;
  float color[3];
  std::string name;
}; 

// ----------------

enum ShapeType {
  T_CIRCLE,
  T_RECTANGLE
};

class Shape {
public:
  std::string name;
  float posX, posY;
  float velX, velY;
  int R, G, B;
  bool draw;
  ShapeType type;

  // -----------------------

  Shape() {}

  Shape(std::string name,
	float posX, float posY,
	float velX, float velY,
	int R, int G, int B,
	bool draw) :
    name(name),
    posX(posX), posY(posY),
    velX(velX), velY(velY),
    R(R), G(G), B(B),
    draw(draw)
  {}

  virtual void drawShape(sf::RenderWindow &window, int fontSize) {}
  virtual void update(UserInput &input, bool selected) {}
};

// Circle Cgreen 100 100 -3 2 0 255 0 50
class Circle : public Shape {
public:
  int radius;
  sf::CircleShape circle; // SFML

  // -----------------------

  Circle() {}

  Circle(std::string name,
	 float posX, float posY,
	 float velX, float velY,
	 int R, int G, int B,
	 bool draw,
	 int radius) :
    Shape(name, posX, posY, velX, velY, R, G, B, draw),
    radius(radius)
  {}

  void drawShape(sf::RenderWindow &window, int fontSize) {
    if (draw) {      
      window.draw(circle);
      
      // NOTE: can you center the text within the shape?
      // NOTE: is this memory being leaked? do we need to explicitly de-allocate it?
      sf::Text text(name.data(), FONT, fontSize);
      text.setPosition(posX + radius, posY + radius);
      window.draw(text);
    } 
  }
  
  void update(UserInput &input, bool selected) {
    // update selected shape
    if (selected) {
      draw = input.draw;

      velX = velX * input.velX;
      velY = velY * input.velY;

      circle.setRadius(radius * input.scale);
      circle.setFillColor(sf::Color(input.color[0] * 255, input.color[1] * 255, input.color[2] * 255));
      
      name = input.name;
    }
    
    posX = posX + velX;
    posY = posY + velY;

    // boundary management
    if (posX + radius >= input.window_width - 1 || posX < 0) { velX *= -1; }
    if (posY + radius >= input.window_height - 1 || posY < 0) { velY *= -1; }

    circle.setPosition(posX, posY);    
  }
  
};

// Rectangle RRed 200 200 4 4 255 0 0 50 25
class Rectangle : public Shape {
public:
  int width, height;
  sf::RectangleShape rectangle; // SFML

  // -----------------------

  Rectangle() {}

  Rectangle(std::string name,
	    float posX, float posY,
	    float velX, float velY,
	    int R, int G, int B,
	    bool draw,
	    int width, int height) :
    Shape(name, posX, posY, velX, velY, R, G, B, draw),
    width(width), height(height)
  {}

  void drawShape(sf::RenderWindow &window, int fontSize) {
    if (draw) {
      window.draw(rectangle);

      // NOTE: can you center the text within the shape?
      // NOTE: is this memory being leaked? do we need to explicitly de-allocate it?
      sf::Text text(name.data(), FONT, fontSize);
      text.setPosition(posX + width/4, posY + height/4);
      window.draw(text);
    }
  }
  
  void update(UserInput &input, bool selected) {
    // update selected shape
    if (selected) {
      draw = input.draw;

      velX = velX + input.velX;
      velY = velY + input.velY;

      rectangle.setSize(sf::Vector2f(width * input.scale, height * input.scale));
      rectangle.setFillColor(sf::Color(input.color[0] * 255, input.color[1] * 255, input.color[2] * 255));
      name = input.name;
    }
    
    posX = posX + velX;
    posY = posY + velY;

    // boundary management
    if (posX + width > input.window_width || posX < 0) { velX *= -1; }
    if (posY + height > input.window_height || posY < 0) { velY *= -1; }

    rectangle.setPosition(posX, posY);
  }
    
};

class Config {
public:
  WindowConfig windowConfig;
  FontConfig fontConfig;
  std::vector<std::shared_ptr<Shape>> shapes;

  Config() {}

  void readFromFile(const std::string& configFilePath) {
    // read values from config file
    std::ifstream configFile(configFilePath);    
    std::stringstream ss;
    std::string val;
    std::string line;
    
    // ------------------
    // Window W H
    std::getline(configFile, line);
    ss << line;    

    std::getline(ss, val, ' '); // skip first value
    std::getline(ss, val, ' '); sscanf(val.c_str(), "%d", &windowConfig.width);
    std::getline(ss, val, ' '); sscanf(val.c_str(), "%d", &windowConfig.height);
    ss.clear();

    // ------------------
    // Font F S R G B
    std::getline(configFile, line);
    ss << line;

    std::getline(ss, val, ' '); // skip first value
    std::getline(ss, val, ' '); fontConfig.path = val;
    std::getline(ss, val, ' '); sscanf(val.c_str(), "%d", &fontConfig.size);
    std::getline(ss, val, ' '); sscanf(val.c_str(), "%d", &fontConfig.R);
    std::getline(ss, val, ' '); sscanf(val.c_str(), "%d", &fontConfig.G);
    std::getline(ss, val, ' '); sscanf(val.c_str(), "%d", &fontConfig.B);
    ss.clear();

    // ------------------
    // Shapes

    while (std::getline(configFile, line)) {
      ss << line;
      
      std::getline(ss, val, ' ');
      
      if (strcmp(val.c_str(), "Circle") == 0) {
	// --------------------------
	// Circle N X Y SX SY R G B R
	//
	// std::cout << "[INFO] - Found circle" << std::endl;
	
	auto c = std::make_shared<Circle>();
	std::getline(ss, val, ' '); c->name = val;
	std::getline(ss, val, ' '); sscanf(val.c_str(), "%f", &c->posX);
	std::getline(ss, val, ' '); sscanf(val.c_str(), "%f", &c->posY);
	std::getline(ss, val, ' '); sscanf(val.c_str(), "%f", &c->velX);
	std::getline(ss, val, ' '); sscanf(val.c_str(), "%f", &c->velY);
	std::getline(ss, val, ' '); sscanf(val.c_str(), "%d", &c->R);
	std::getline(ss, val, ' '); sscanf(val.c_str(), "%d", &c->G);
	std::getline(ss, val, ' '); sscanf(val.c_str(), "%d", &c->B);
	std::getline(ss, val, ' '); sscanf(val.c_str(), "%d", &c->radius);

	// std::cout << c->velX << std::endl;

	c->draw = true;
	c->type = ShapeType::T_CIRCLE;

	shapes.push_back(c);
      }
      else if (strcmp(val.c_str(), "Rectangle") == 0) {
	// -------------------------------
	// Rectangle N X Y SX SY R G B W H
	// 
	// std::cout << "[INFO] - Found Rectangle" << std::endl;

	auto r = std::make_shared<Rectangle>();
	std::getline(ss, val, ' '); r->name = val;
	std::getline(ss, val, ' '); sscanf(val.c_str(), "%f", &r->posX);
	std::getline(ss, val, ' '); sscanf(val.c_str(), "%f", &r->posY);
	std::getline(ss, val, ' '); sscanf(val.c_str(), "%f", &r->velX);
	std::getline(ss, val, ' '); sscanf(val.c_str(), "%f", &r->velY);
	std::getline(ss, val, ' '); sscanf(val.c_str(), "%d", &r->R);
	std::getline(ss, val, ' '); sscanf(val.c_str(), "%d", &r->G);
	std::getline(ss, val, ' '); sscanf(val.c_str(), "%d", &r->B);
	std::getline(ss, val, ' '); sscanf(val.c_str(), "%d", &r->width);
	std::getline(ss, val, ' '); sscanf(val.c_str(), "%d", &r->height);	

	r->draw = true;
	r->type = ShapeType::T_RECTANGLE;

	shapes.push_back(r);
      }
      else {
	std::cerr << "[ERROR]: Shape " << val << " is not supported!" << std::endl;
	exit(1);
      }
      ss.clear();
    }
  }
};

class Game {
public:
  UserInput input;
  Config config;

  Game(const std::string configFile) {
    config.readFromFile(configFile);

    if(!FONT.loadFromFile(config.fontConfig.path)) {
      std::cerr << std::format("[ERROR] - Could not load font '{}'", config.fontConfig.path) << std::endl;
      exit(1);
    }

    initShapes();
    initUserInput();
  }

  void initShapes(void) {
    // iterate over all shape read in config and for each one
    // instantiate an element using SFML
    // 
    for (auto shape : config.shapes) {

      switch(shape->type) {
      case ShapeType::T_RECTANGLE: {
	auto r = std::dynamic_pointer_cast<Rectangle>(shape);
	r->rectangle.setSize(sf::Vector2f(r->width, r->height));
	r->rectangle.setPosition(r->posX, r->posY);
	r->rectangle.setFillColor(sf::Color(r->R, r->G, r->B));			
	break;
      }	
	
      case ShapeType::T_CIRCLE: {
	auto c = std::dynamic_pointer_cast<Circle>(shape);
	c->circle.setRadius(c->radius);
	c->circle.setPointCount(32);
	c->circle.setPosition(c->posX, c->posY);
	c->circle.setFillColor(sf::Color(c->R, c->G, c->B));		
	break;
      }
      }
    }
  }

  void initUserInput(void) {
    input.window_width = config.windowConfig.width;
    input.window_height = config.windowConfig.height;
    
    input.scale = 1.0f;
    input.velX = 1.0f;
    input.velY = 1.0f;

    input.name = config.shapes[0]->name;
    input.draw = config.shapes[0]->draw;
    input.color[0] = config.shapes[0]->R / 255.0f;
    input.color[1] = config.shapes[0]->G / 255.0f;
    input.color[2] = config.shapes[0]->B / 255.0f; 
  }

  void updateInput(int selected_shape_index) {
    input.name = config.shapes[selected_shape_index]->name;
    input.draw = config.shapes[selected_shape_index]->draw;
    
    input.scale = 1.0f;
    input.velX = 1.0f;
    input.velY = 1.0f;
    
    input.color[0] = config.shapes[selected_shape_index]->R / 255.0f;
    input.color[1] = config.shapes[selected_shape_index]->G / 255.0f;
    input.color[2] = config.shapes[selected_shape_index]->B / 255.0f;
  }
  
};

// ---------------------------------------------------------------

int main(void) {
  Game g = Game(CONFIG_FILE);
  
  const int window_W = g.config.windowConfig.width;
  const int window_H = g.config.windowConfig.height;
  sf::RenderWindow window(sf::VideoMode(window_W, window_H), WINDOW_TITLE);
  window.setFramerateLimit(60);

  // imgui init
  ImGui::SFML::Init(window);
  ImGui::GetStyle().ScaleAllSizes(1.0f);
  sf::Clock deltaClock;
  static ImGuiComboFlags combo_flags = 0;

  int selected_shape_index = 0;
  char *selected_shape_name = NULL;
  
  // Game loop
  while (window.isOpen()) {
    
    // -----------------------------
    // EVENT HANDLING
    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(window, event);
      if (event.type == sf::Event::Closed) {
	window.close();
      }
    }

    // -----------------------------
    // GAME UPDATE

    for (int i = 0; i < g.config.shapes.size(); i++) {
      g.config.shapes[i]->update(g.input, selected_shape_index == i);
    }

    // -----------------------------
    // DRAW IMGUI
    
    ImGui::SFML::Update(window, deltaClock.restart());
    ImGui::Begin(IMGUI_TITLE);

    // show list of currently available shapes to select one
    selected_shape_name = g.config.shapes[selected_shape_index]->name.data();
    if (ImGui::BeginCombo("Shape Selection", selected_shape_name, combo_flags)) {
      for (int i = 0; i < g.config.shapes.size(); i++) {
	char *name = g.config.shapes[i]->name.data();
	if (ImGui::Selectable(name)) {
	  selected_shape_index = i;
	  g.updateInput(selected_shape_index);
	}
      }
      ImGui::EndCombo();
    }

    ImGui::Checkbox("Draw Shape", &g.input.draw);
    ImGui::SliderFloat("Scale", &g.input.scale, 0.0f, 4.0f);
    ImGui::SliderFloat("VelX", &g.input.velX, 0.0f, 4.0f);
    ImGui::SliderFloat("VelY", &g.input.velY, 0.0f, 4.0f);
    ImGui::ColorEdit3("Color", g.input.color);
    ImGui::InputText("Name", g.input.name.data(), 255);    
    
    ImGui::End();
    
    // -----------------------------
    // RENDERING
    window.clear();
    for (auto shape : g.config.shapes) {
      shape->drawShape(window, g.config.fontConfig.size);
    }
    ImGui::SFML::Render(window);
    window.display();
  }
  
  return 0;
}
