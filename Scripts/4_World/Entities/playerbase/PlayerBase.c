modded class PlayerBase
{
	override void Init()
	{
		DayzPlayerItemBehaviorCfg heavyItemBehaviour = new DayzPlayerItemBehaviorCfg;
		heavyItemBehaviour.SetHeavyItems();
		GetDayZPlayerType().AddItemInHandsProfileIK(
		"Printer3D_Printer",
		"dz/anims/workspaces/player/player_main/player_main_heavy.asi",
		heavyItemBehaviour,
		"dz/anims/anm/player/ik/heavy/sea_chest.anm");

		GetDayZPlayerType().AddItemInHandsProfileIK(
		"Printer3D_FilingCabinet",
		"dz/anims/workspaces/player/player_main/player_main_heavy.asi",
		heavyItemBehaviour,
		"dz/anims/anm/player/ik/heavy/sea_chest.anm");

		GetDayZPlayerType().AddItemInHandsProfileIK(
		"Printer3D_KitBase",
		"dz/anims/workspaces/player/player_main/player_main_heavy.asi",
		heavyItemBehaviour,
		"dz/anims/anm/player/ik/heavy/sea_chest.anm");

		super.Init();

		GetRPCManager().AddRPC("Printer3D", "SendTranslatedReplacedString", this, SingleplayerExecutionType.Client);
	}

	string TranslateString(string key)
	{
		return (new CF_Localiser(key)).Format();
	}

	void SendTranslatedReplacedString(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        Param3<string, string, PlayerIdentity> param;
        if(!ctx.Read(param)) return;

		string translatedString = TranslateString(param.param1);
		translatedString.Replace("{0}", param.param2);
		GetRPCManager().SendRPC("Printer3D", "SendPrinterNotification", new Param2<string, PlayerIdentity>(translatedString, param.param3), true);
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(OpenPrinterMenu);
		AddAction(DismantlePrinterObjects);
	}
};