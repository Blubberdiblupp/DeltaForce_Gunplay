class DeltaForceGunplayOwnership
{
	static bool IsModLoaded(string modName)
	{
		return GetGame().ConfigIsExisting("CfgPatches " + modName);
	}
	
	static bool OwnsCommonGunplay()
	{
		if (IsModLoaded("SNAFU_Gunplay")) return false;
		if (IsModLoaded("AJ_Gunplay")) return false;
		if (IsModLoaded("Mortys_Gunplay")) return false;
		if (IsModLoaded("Vanilla_Gunplay")) return false;
		if (IsModLoaded("Expansion_Gunplay")) return false;
		
		return true;
	}
	
	static bool IsDeltaForceClass(string className)
	{
		if (className == "") return false;
		if (className.Contains("My_DF_Weapons_")) return true;
		
		return false;
	}
	
	static bool ShouldOwnItemType(string itemType, array<string> attachmentTypes = null)
	{
		if (IsDeltaForceClass(itemType))
		{
			return true;
		}
		
		if (attachmentTypes)
		{
			foreach (string attachmentType : attachmentTypes)
			{
				if (IsDeltaForceClass(attachmentType))
				{
					return true;
				}
			}
		}
		
		return OwnsCommonGunplay();
	}
}