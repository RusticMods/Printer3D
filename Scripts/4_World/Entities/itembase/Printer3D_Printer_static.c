class Printer3D_Printer_static: Printer3D_Printer
{
    bool RGFS = false;

    override bool CanPutIntoHands(EntityAI parent)
	{
        return false;
	}
    override bool CanPutInCargo( EntityAI parent )
    {
        return false;
    }

    void Printer3D_Printer_static() 
    {   
        if (!GetGame().IsMultiplayer() || !GetGame().IsServer()) { return; }
        
        if (GetDayZGame().GetPrinterConfig().RGFS()) {RGFS = true;}   
    }

    bool RGFS() {return RGFS;}
}
