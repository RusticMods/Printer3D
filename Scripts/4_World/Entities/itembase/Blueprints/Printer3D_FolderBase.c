class Printer3D_FolderBase: ItemBase
{
	
	string m_SelectedEntityName;
    string m_SelectedEntityDisplayName;
	
	int m_SelectedEntityTimeToPrint;
	int m_SelectedEntityFilamentAmount;
	int m_SelectedEntityFilamentTier;

	int m_SelectedEntityPrintQuantity;

    void Printer3D_FolderBase()
    {
		m_SelectedEntityName = GetStringFromConfig(this.GetType(), "printerPrintTypename");
        m_SelectedEntityDisplayName = GetStringFromConfig(this.GetType(), "printerDisplayname");
	
		m_SelectedEntityTimeToPrint = GetStringFromConfig(this.GetType(), "printerTimeToPrint").ToInt();
		m_SelectedEntityFilamentAmount = GetStringFromConfig(this.GetType(), "printerFilamentAmount").ToInt();
		m_SelectedEntityFilamentTier = GetStringFromConfig(this.GetType(), "printerFilamentTier").ToInt();

		m_SelectedEntityPrintQuantity = GetStringFromConfig(this.GetType(), "printerPrintQuantity").ToInt();
	}

	override bool DescriptionOverride(out string output)
	{
		string description = "3D Model Folder - Stored Model: " + m_SelectedEntityDisplayName;

		if (description != string.Empty)
		{
			output = description;
			return true;
		}
		
		return false;
	}

	override bool NameOverride(out string output)
	{
		output = "3D Model Folder" + " - " + m_SelectedEntityDisplayName;
		return true;
	}
	
	//
	//Helper Functions
	//

	string GetStringFromConfig(string itemName, string configName)
	{
		if (GetGame().ConfigIsExisting("CfgVehicles " + itemName + " " + configName))
		{
			return ConfigGetString(configName);
		}
		return "";
	}
	
    string GetSelectedEntityName() { return m_SelectedEntityName; }
    string GetSelectedEntityDisplayName() { return m_SelectedEntityDisplayName; }
	
	int GetSelectedEntityTimeToPrint() { return m_SelectedEntityTimeToPrint; }
	int GetSelectedEntityFilamentAmount() { return m_SelectedEntityFilamentAmount; }
	int GetSelectedEntityFilamentTier() { return m_SelectedEntityFilamentTier; }

	int GetSelectedEntityPrintQuantity() { return m_SelectedEntityPrintQuantity; }
	
};