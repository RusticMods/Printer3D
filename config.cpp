class CfgMods
{
	class Printer3D
	{
		dir = "Printer3D";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "Printer3D";
		credits = "Rustic";
		author = "Rustic";
		authorID = "69";
		extra = 0;
		type = "mod";
		dependencies[] = {"Game","World","Mission"};
		class defs
		{
			class imageSets
			{
				files[] = {"Printer3D/Data/GUI/imagesets/printer_icons.imageset"};
			};

			class gameScriptModule
			{
				value = "";
				files[] = {"Printer3D/Scripts/3_Game"};
			};

			class worldScriptModule
			{
				value = "";
				files[] = {"Printer3D/Scripts/4_World"};
			};

			class missionScriptModule
			{
				value = "";
				files[] = {"Printer3D/Scripts/5_Mission"};
			};
		};
	};
};
class CfgPatches
{
	class Printer3D
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Scripts","JM_CF_Scripts","RusticModsCore"};
	};
};

class CfgNonAIVehicles
{
	class ProxyAttachment;
	class ProxyUSB : ProxyAttachment
	{
		scope=2;
		inventorySlot="Printer3D_USB";
		model="Printer3D/Data/Proxies/USB.p3d";
	};

	class ProxyFilament : ProxyAttachment
	{
		scope=2;
		inventorySlot="Filament";
		model="Printer3D/Data/Proxies/Filament.p3d";
	};
};

class CfgSlots
{
	class slot_printer3d_usb
	{
		name = "Printer3D_USB";
		displayName = "#str_usb";
		ghostIcon = "set:printer_icons image:usb_icon";
	};

	class slot_filament
	{
		name = "Filament";
		displayName = "#str_filament";
		ghostIcon = "set:printer_icons image:filament_icon";
	};

	class slot_folders1{ name = "Folders1"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders2{ name = "Folders2"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders3{ name = "Folders3"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders4{ name = "Folders4"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders5{ name = "Folders5"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders6{ name = "Folders6"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders7{ name = "Folders7"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders8{ name = "Folders8"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders9{ name = "Folders9"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders10{ name = "Folders10"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders11{ name = "Folders11"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders12{ name = "Folders12"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders13{ name = "Folders13"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders14{ name = "Folders14"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders15{ name = "Folders15"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders16{ name = "Folders16"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders17{ name = "Folders17"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders18{ name = "Folders18"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders19{ name = "Folders19"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders20{ name = "Folders20"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders21{ name = "Folders21"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders22{ name = "Folders22"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders23{ name = "Folders23"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};
	class slot_folders24{ name = "Folders24"; displayName = "#str_3dmodelfolder"; ghostIcon = "set:printer_icons image:folder_icon";};

	class slot_folder1
	{
		name = "Folder1";
		displayName = "#str_3dmodelfolder";
		ghostIcon = "set:printer_icons image:folder_icon";
	};

	class slot_folder2
	{
		name = "Folder2";
		displayName = "#str_3dmodelfolder";
		ghostIcon = "set:printer_icons image:folder_icon";
	};

	class slot_folder3
	{
		name = "Folder3";
		displayName = "#str_3dmodelfolder";
		ghostIcon = "set:printer_icons image:folder_icon";
	};

	class slot_folder4
	{
		name = "Folder4";
		displayName = "#str_3dmodelfolder";
		ghostIcon = "set:printer_icons image:folder_icon";
	};


};

class CfgVehicles
{
	class Container_Base;
	class Inventory_Base;
	
	//---------------------------------------//
	//------------------Kits-----------------//
	//---------------------------------------//

	class Printer3D_KitBase: Inventory_Base
	{
		scope=0;
		model="Printer3D\Data\Kit\Kit.p3d";
		displayName = "#str_3dprinterkitbase";
		descriptionShort = "#str_baseentitywarning";
		slopeTolerance = 0.3;
		itemBehaviour = 0;
		physLayer="item_large";
		itemSize[] = {5,4};
		weight = 10000;
	};

	class Printer3D_Printer_Kit : Printer3D_KitBase
	{
		scope=2;
		displayName = "#str_3dprinterkit";
		descriptionShort = "#str_3dprinterkitdesc";
		ProjectionTypename="Printer3D_Printer"
		rotationFlags = 0;
	};

	class Printer3D_Printer_1000S_Kit : Printer3D_KitBase
	{
		scope=2;
		displayName = "#str_3dprinterkit1000";
		descriptionShort = "#str_3dprinterkit1000desc";
		ProjectionTypename="Printer3D_Printer_1000_Slots"
	};

	class Printer3D_FilingCabinet_Kit : Printer3D_KitBase
	{
		scope=2;
		displayName = "#str_filingcabinetkit";
		descriptionShort = "#str_filingcabinetkitdesc";
		ProjectionTypename="Printer3D_FilingCabinet"
	};


	//---------------------------------------//
	//---------------Printers----------------//
	//---------------------------------------//

	class Printer3D_USB: Inventory_Base
	{
		scope=2;
		model="Printer3D\Data\BPUSB\BPUSB.p3d";
		displayName = "#str_3dmodelusb";
		descriptionShort = "#str_3dmodelusbdesc";
		slopeTolerance = 0.3;
		itemBehaviour = 1;

		inventorySlot[] = {"Printer3D_USB"};
		attachments[]=
		{
			"Folder1",
			"Folder2",
			"Folder3",
			"Folder4",
		};

		class Cargo
		{
			allowOwnedCargoManipulation=1;
		};
	};

	class Printer3D_Printer: Inventory_Base
	{
		model = "Printer3D\Data\Printer3D\Printer.p3d";
		displayName = "#str_3dprinter";
		descriptionShort = "#str_3dprinterdesc";

		itemSize[] = {5,5};

		physLayer="item_large";
		scope = 2;
		rotationFlags = 0;
		weight = 10000;
		hiddenSelections[] = {"placing", "Printer"};
		hiddenSelectionsTextures[] = {"Printer3D\Data\Printer3D\Printer_albedo_REDPrinter.paa", "Printer3D\Data\Printer3D\Printer_albedo_REDPrinter.paa"};
		hiddenSelectionsMaterials[] = {"dz/gear/camping/data/fence_pile_of_planks.rvmat", "dz/gear/camping/data/fence_pile_of_planks.rvmat"};
		hologramMaterial = "tent_medium";
		hologramMaterialPath = "dz/gear/camping/data";
		alignHologramToTerrain = 1;
		slopeTolerance = 0.3;
		itemBehaviour = 0;
		class EnergyManager
		{
			hasIcon = 1;
			powerSocketsCount = 1;
			energyUsagePerSecond = 0.3;
			cordTextureFile = "DZ/gear/camping/Data/plug_black_CO.paa";
			cordLength = 5;
			plugType = 0;
		};
		attachments[]=
		{
			"Filament",
			"Printer3D_USB",
		};
		class Cargo
		{
			itemsCargoSize[]={10,10};
			openable=0;
			allowOwnedCargoManipulation=1;
		};
		class GUIInventoryAttachmentsProps
		{
			class Filament
			{
				name = "#str_filament";
				description = "";
				attachmentSlots[] = {"Filament"};
				icon = "";
				view_index = 1;
			};

			class Printer3D_USB
			{
				name = "#str_3dmodelusb";
				description = "";
				attachmentSlots[] = {"Printer3D_USB"};
				icon = "";
				view_index = 1;
			};
		};
	};
	class Printer3D_Printer_static: Printer3D_Printer {};

	class Printer3D_Printer_1000_Slots: Printer3D_Printer
	{
		class Cargo
		{
			itemsCargoSize[]={10,100};
			openable=0;
			allowOwnedCargoManipulation=1;
		};
	};

	class Printer3D_Printer_static_1000_Slots: Printer3D_Printer_static
	{
		class Cargo
		{
			itemsCargoSize[]={10,100};
			openable=0;
			allowOwnedCargoManipulation=1;
		};
	};

	class Printer3D_FilingCabinet: Container_Base
	{
		model = "Printer3D\Data\FilingCabinet\filingcabinet.p3d";
		displayName = "#str_filingcabinet";
		descriptionShort = "#str_filingcabinetdesc";
		
		itemSize[] = {7,13};

		scope = 2;
		weight = 10000;
		hiddenSelections[] = {"placing"};
		hiddenSelectionsTextures[] = {"dz/gear/consumables/data/pile_of_planks_co.paa"};
		hiddenSelectionsMaterials[] = {"dz/gear/camping/data/fence_pile_of_planks.rvmat"};
		hologramMaterial = "tent_medium";
		hologramMaterialPath = "dz/gear/camping/data";
		alignHologramToTerrain = 1;
		slopeTolerance = 0.3;
		itemBehaviour = 0;
		physLayer="item_large";
		class Cargo
		{
			itemsCargoSize[]={10,5};
			openable=0;
			allowOwnedCargoManipulation=1;
		};
		attachments[]={ "Folders1","Folders2","Folders3","Folders4","Folders5","Folders6","Folders7","Folders8","Folders9","Folders10","Folders11","Folders12","Folders13","Folders14","Folders15","Folders16","Folders17","Folders18","Folders19","Folders20","Folders21","Folders22","Folders23","Folders24" };
		class GUIInventoryAttachmentsProps
		{
			class Drawer1
			{
				name = "#str_drawer1";
				description = "";
				attachmentSlots[] = {"Folders1","Folders2","Folders3","Folders4","Folders5","Folders6","Folders7","Folders8"};
				icon = "set:printer_icons image:folder_icon";
				view_index = 1;
			};

			class Drawer2
			{
				name = "#str_drawer2";
				description = "";
				attachmentSlots[] = {"Folders9","Folders10","Folders11","Folders12","Folders13","Folders14","Folders15","Folders16"};
				icon = "set:printer_icons image:folder_icon";
				view_index = 1;
			};

			class Drawer3
			{
				name = "#str_drawer3";
				description = "";
				attachmentSlots[] = {"Folders17","Folders18","Folders19","Folders20","Folders21","Folders22","Folders23","Folders24"};
				icon = "set:printer_icons image:folder_icon";
				view_index = 1;
			};
		};
	};

	//---------------------------------------//
	//-------------Base Entities-------------//
	//---------------------------------------//

	class Printer3D_FilamentBase: Inventory_Base
	{
		scope = 0;
		weight = 25;
		model = "Printer3D\Data\PrinterFilament\Filament.p3d";
		itemSize[] = {2,2};
		destroyOnEmpty = 1;
		varQuantityDestroyOnMin = 1;
		varQuantityInit = 1000.0;
		varQuantityMin = 0.0;
		varQuantityMax = 1000.0;
		canBeSplit=1;

		inventorySlot[] = {"Filament"};
	};

	class Printer3D_FolderBase: Inventory_Base
	{
		scope = 0;
		weight = 25;
		model = "Printer3D\Data\FileFolder\Folder.p3d";
		itemSize[] = {2,2};
		destroyOnEmpty = 1;
		varQuantityDestroyOnMin = 1;
		quantityBar = 1;

		inventorySlot[] = 
		{
			"Folders1","Folders2","Folders3","Folders4","Folders5","Folders6","Folders7","Folders8","Folders9","Folders10","Folders11","Folders12","Folders13","Folders14","Folders15","Folders16","Folders17","Folders18","Folders19","Folders20","Folders21","Folders22","Folders23","Folders24",
			"Folder1", "Folder2", "Folder3", "Folder4"
		};
	};
};
