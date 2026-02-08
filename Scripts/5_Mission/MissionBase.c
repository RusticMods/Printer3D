modded class MissionBase {
    override UIScriptedMenu CreateScriptedMenu(int id) {
        UIScriptedMenu menu = NULL;
        menu = super.CreateScriptedMenu(id);
        if (!menu) {
            switch (id) {
            case PRINTER_MENU:
                menu = new PrinterMenu;
                break;
            }
            if (menu) {
                menu.SetID(id);
            }
        }
        return menu;
    }
}