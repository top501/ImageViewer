// generated by Fast Light User Interface Designer (fluid) version 1.0009

#include "CellsViewerGUI.h"

inline void CellsViewerGUI::cb_Quit_i(Fl_Menu_*, void*) {
  Quit();
}
void CellsViewerGUI::cb_Quit(Fl_Menu_* o, void* v) {
  ((CellsViewerGUI*)(o->parent()->user_data()))->cb_Quit_i(o,v);
}

Fl_Menu_Item CellsViewerGUI::menu_[] = {
 {"File", 0,  0, 0, 64, 0, 0, 14, 0},
 {"Quit", 0x80071,  (Fl_Callback*)CellsViewerGUI::cb_Quit, 0, 0, 0, 0, 14, 0},
 {0},
 {0}
};

inline void CellsViewerGUI::cb_3_i(Fl_Button*, void*) {
  Run();
}
void CellsViewerGUI::cb_3(Fl_Button* o, void* v) {
  ((CellsViewerGUI*)(o->parent()->user_data()))->cb_3_i(o,v);
}

inline void CellsViewerGUI::cb_4square_i(Fl_Button*, void*) {
  Stop();
}
void CellsViewerGUI::cb_4square(Fl_Button* o, void* v) {
  ((CellsViewerGUI*)(o->parent()->user_data()))->cb_4square_i(o,v);
}

inline void CellsViewerGUI::cb__i(Fl_Button*, void*) {
  Restart();
}
void CellsViewerGUI::cb_(Fl_Button* o, void* v) {
  ((CellsViewerGUI*)(o->parent()->user_data()))->cb__i(o,v);
}

CellsViewerGUI::CellsViewerGUI() {
  Fl_Window* w;
  { Fl_Window* o = controlWindow = new Fl_Window(172, 192, "Control Panel");
    w = o;
    o->user_data((void*)(this));
    { Fl_Menu_Bar* o = new Fl_Menu_Bar(0, 0, 170, 25);
      o->menu(menu_);
    }
    { Fl_Button* o = new Fl_Button(15, 40, 45, 20, "@-3>");
      o->labeltype(FL_SYMBOL_LABEL);
      o->callback((Fl_Callback*)cb_3);
    }
    { Fl_Button* o = new Fl_Button(60, 40, 45, 20, "@#-4square");
      o->labeltype(FL_SYMBOL_LABEL);
      o->callback((Fl_Callback*)cb_4square);
    }
    iterationValueOutput = new Fl_Value_Output(85, 70, 60, 25, "Iteration : ");
    numberOfCellsValueOutput = new Fl_Value_Output(85, 100, 60, 25, "Cells : ");
    { Fl_Button* o = new Fl_Button(105, 40, 45, 20, "@#|<");
      o->labeltype(FL_SYMBOL_LABEL);
      o->callback((Fl_Callback*)cb_);
    }
    timeValueOutput = new Fl_Value_Output(85, 130, 60, 25, "Time : ");
    o->end();
  }
}

CellsViewerGUI::~CellsViewerGUI() {
}
