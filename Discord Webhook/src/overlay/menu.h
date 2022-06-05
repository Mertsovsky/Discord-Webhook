#ifndef _MENU_HEADER_
#define _MENU_HEADER_

#define MENU_SIZE ImVec2(640, 480)

namespace wavvve::overlay {
	class Menu {
	public:
		void Render();
	private:

	};
}

extern std::unique_ptr<wavvve::overlay::Menu> g_pMenu;

#endif