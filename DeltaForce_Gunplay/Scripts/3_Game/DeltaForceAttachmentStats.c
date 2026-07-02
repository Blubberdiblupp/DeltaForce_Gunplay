class DeltaForceAttachmentStats
{
	protected static ref map<string, string> m_SlotTierFallbacks;
	
	static DeltaForceTierEffect GetEffect(EntityAI item)
	{
		string tierKey = GetTierKey(item);
		if (tierKey == "")
		{
			return null;
		}
		
		return DeltaForceGunplayConfig.GetInstance().GetTierEffect(tierKey);
	}
	
	static DeltaForceTierEffect GetEffectByType(string itemType)
	{
		string tierKey = GetTierKeyByType(itemType);
		if (tierKey == "")
		{
			return null;
		}
		
		return DeltaForceGunplayConfig.GetInstance().GetTierEffect(tierKey);
	}
	
	static string GetTierKey(EntityAI item)
	{
		if (!item)
		{
			return "";
		}
		
		DeltaForceGunplayConfig config = DeltaForceGunplayConfig.GetInstance();
		if (!config)
		{
			return "";
		}
		
		string tierKey = config.GetAttachmentTier(item.GetType(), "");
		if (tierKey != "")
		{
			return tierKey;
		}
		
		int magCount = GetGame().ConfigGetInt("CfgMagazines " + item.GetType() + " count");
		if (magCount > config.HighCapMagazineThreshold)
		{
			return "HighCap_Heavy";
		}
		
		if (magCount > 0)
		{
			return "StandardMag_Neutral";
		}
		
		return GetTierKeyFromInventorySlot(item);
	}
	
	static string GetTierKeyByType(string itemType)
	{
		if (itemType == "")
		{
			return "";
		}
		
		DeltaForceGunplayConfig config = DeltaForceGunplayConfig.GetInstance();
		if (!config)
		{
			return "";
		}
		
		string tierKey = config.GetAttachmentTier(itemType, "");
		if (tierKey != "")
		{
			return tierKey;
		}
		
		int magCount = GetGame().ConfigGetInt("CfgMagazines " + itemType + " count");
		if (magCount > config.HighCapMagazineThreshold)
		{
			return "HighCap_Heavy";
		}
		
		if (magCount > 0)
		{
			return "StandardMag_Neutral";
		}
		
		return GetTierKeyFromInventorySlotByType(itemType);
	}
	
	static string GetTierKeyFromInventorySlot(EntityAI item)
	{
		if (!item)
		{
			return "";
		}
		
		EnsureSlotTierFallbacks();
		
		TStringArray slots = new TStringArray;
		item.ConfigGetTextArray("inventorySlot", slots);
		
		foreach (string slot : slots)
		{
			string tierKey = ResolveTierFromSlot(slot);
			if (tierKey != "")
			{
				return tierKey;
			}
		}
		
		string singleSlot = item.ConfigGetString("inventorySlot");
		return ResolveTierFromSlot(singleSlot);
	}
	
	static string GetTierKeyFromInventorySlotByType(string itemType)
	{
		if (itemType == "")
		{
			return "";
		}
		
		EnsureSlotTierFallbacks();
		
		TStringArray slots = new TStringArray;
		GetGame().ConfigGetTextArray("CfgVehicles " + itemType + " inventorySlot", slots);
		
		foreach (string slot : slots)
		{
			string tierKey = ResolveTierFromSlot(slot);
			if (tierKey != "")
			{
				return tierKey;
			}
		}
		
		string singleSlot;
		GetGame().ConfigGetText("CfgVehicles " + itemType + " inventorySlot", singleSlot);
		return ResolveTierFromSlot(singleSlot);
	}
	
	static string ResolveTierFromSlot(string slotName)
	{
		if (slotName == "")
		{
			return "";
		}
		
		EnsureSlotTierFallbacks();
		
		if (m_SlotTierFallbacks.Contains(slotName))
		{
			return m_SlotTierFallbacks.Get(slotName);
		}
		
		return "";
	}
	
	static string BuildTooltipText(EntityAI item, string tooltip)
	{
		string statsText = BuildStatsText(item);
		if (statsText == "")
		{
			return tooltip;
		}
		
		if (tooltip == "")
		{
			return statsText;
		}
		
		return tooltip + "\n\n" + statsText;
	}
	
	static string BuildStatsText(EntityAI item)
	{
		DeltaForceTierEffect effect = GetEffect(item);
		if (!effect)
		{
			return "";
		}
		
		string text = "DeltaForce Gunplay\n";
		text += "Tier: " + effect.GetLabel() + "\n";
		text += "Recoil: " + FormatDelta(effect.Recoil) + "\n";
		text += "Sway: " + FormatDelta(effect.Sway) + "\n";
		text += "ADS: " + FormatDelta(effect.ADS) + "\n";
		text += "Precision: " + FormatDelta(effect.Precision) + "\n";
		text += "HipFire: " + FormatDelta(effect.HipFire);
		
		return text;
	}
	
	static string FormatDelta(float multiplier)
	{
		if (Math.AbsFloat(multiplier - 1.0) < 0.001)
		{
			return "0%";
		}
		
		int rounded = Math.Round((multiplier - 1.0) * 100.0);
		if (rounded > 0)
		{
			return string.Format("+%1%%", rounded);
		}
		
		return string.Format("%1%%", rounded);
	}
	
	static int GetDeltaColor(float multiplier, bool higherIsBetter)
	{
		if (Math.AbsFloat(multiplier - 1.0) < 0.001)
		{
			return 0xFFFFFFFF;
		}
		
		bool improved;
		if (higherIsBetter)
		{
			improved = multiplier > 1.0;
		}
		else
		{
			improved = multiplier < 1.0;
		}
		
		if (improved)
		{
			return 0xFF22DD66;
		}
		
		return 0xFFFF5555;
	}
	
	protected static void EnsureSlotTierFallbacks()
	{
		if (m_SlotTierFallbacks)
		{
			return;
		}

		m_SlotTierFallbacks = new map<string, string>;

		AddSlotTier("22LRSuppressor", "Suppressor_T1");
		AddSlotTier("ASH12Suppressor", "Suppressor_T2");
		AddSlotTier("DF_ak_handlecap", "Handguard_T1");
		AddSlotTier("DF_ak_pisg", "PistolGrip_T1");
		AddSlotTier("DF_ar_pisg", "PistolGrip_T1");
		AddSlotTier("DF_ar_pisg_ext", "PistolGrip_T1");
		AddSlotTier("DF_barrel_93R", "Muzzle_T1");
		AddSlotTier("DF_barrel_AKM", "Muzzle_T1");
		AddSlotTier("DF_barrel_Ash12", "Muzzle_T1");
		AddSlotTier("DF_barrel_AWM", "Muzzle_T1");
		AddSlotTier("DF_barrel_Bison", "Muzzle_T1");
		AddSlotTier("DF_barrel_ColtPython", "Muzzle_T1");
		AddSlotTier("DF_barrel_Deagle", "Muzzle_T1");
		AddSlotTier("DF_barrel_FS12", "Muzzle_T1");
		AddSlotTier("DF_barrel_G17", "Muzzle_T1");
		AddSlotTier("DF_barrel_G18", "Muzzle_T1");
		AddSlotTier("DF_barrel_M1014", "Muzzle_T1");
		AddSlotTier("DF_barrel_M14", "Muzzle_T1");
		AddSlotTier("DF_barrel_M1911", "Muzzle_T1");
		AddSlotTier("DF_barrel_M249", "Muzzle_T1");
		AddSlotTier("DF_barrel_M250", "Muzzle_T1");
		AddSlotTier("DF_barrel_M700", "Muzzle_T1");
		AddSlotTier("DF_barrel_M870", "Muzzle_T1");
		AddSlotTier("DF_barrel_Mini14", "Muzzle_T1");
		AddSlotTier("DF_barrel_MP7", "Muzzle_T1");
		AddSlotTier("DF_barrel_P90", "Muzzle_T1");
		AddSlotTier("DF_barrel_PKM", "Muzzle_T1");
		AddSlotTier("DF_barrel_PP19", "Muzzle_T1");
		AddSlotTier("DF_barrel_QBZ191", "Muzzle_T1");
		AddSlotTier("DF_barrel_QBZ95", "Muzzle_T1");
		AddSlotTier("DF_barrel_QJB201", "Muzzle_T1");
		AddSlotTier("DF_barrel_QSZ92G", "Muzzle_T1");
		AddSlotTier("DF_barrel_R93", "Muzzle_T1");
		AddSlotTier("DF_barrel_SG553", "Muzzle_T1");
		AddSlotTier("DF_barrel_SKS", "Muzzle_T1");
		AddSlotTier("DF_barrel_SMG45", "Muzzle_T1");
		AddSlotTier("DF_barrel_SR25", "Muzzle_T1");
		AddSlotTier("DF_barrel_SV98", "Muzzle_T1");
		AddSlotTier("DF_barrel_SVD", "Muzzle_T1");
		AddSlotTier("DF_barrel_Uzi", "Muzzle_T1");
		AddSlotTier("DF_barrel_VSS", "Muzzle_T1");
		AddSlotTier("DF_bipod", "Bipod_Special");
		AddSlotTier("DF_bipod_AK12", "Bipod_Special");
		AddSlotTier("DF_bipod_AUG", "Bipod_Special");
		AddSlotTier("DF_bipod_AWM", "Bipod_Special");
		AddSlotTier("DF_bipod_M249", "Bipod_Special");
		AddSlotTier("DF_bipod_M250", "Bipod_Special");
		AddSlotTier("DF_bipod_PKM", "Bipod_Special");
		AddSlotTier("DF_bipod_QJB201", "Bipod_Special");
		AddSlotTier("DF_bipod_SV98", "Bipod_Special");
		AddSlotTier("DF_Bolt_QCQ171", "Handguard_T1");
		AddSlotTier("DF_cheekpad", "Stock_T2");
		AddSlotTier("DF_cheekpad_QBZ95", "Stock_T2");
		AddSlotTier("DF_Common", "Handguard_T1");
		AddSlotTier("DF_flashkill", "Flashlight_Utility");
		AddSlotTier("DF_frog_down", "Foregrip_T1");
		AddSlotTier("DF_frog_left", "Foregrip_T1");
		AddSlotTier("DF_frog_left2", "Foregrip_T1");
		AddSlotTier("DF_frog_right", "Foregrip_T1");
		AddSlotTier("DF_frog_right2", "Foregrip_T1");
		AddSlotTier("DF_frog_up", "Foregrip_T1");
		AddSlotTier("DF_frog_up2", "Foregrip_T1");
		AddSlotTier("DF_GrenadeAmmo", "Handguard_T1");
		AddSlotTier("DF_GrenadeAmmo2", "Handguard_T1");
		AddSlotTier("DF_GrenadeAmmo3", "Handguard_T1");
		AddSlotTier("DF_GrenadeAmmo4", "Handguard_T1");
		AddSlotTier("DF_GrenadeAmmo5", "Handguard_T1");
		AddSlotTier("DF_GrenadeAmmo6", "Handguard_T1");
		AddSlotTier("DF_hammer_M1911", "PistolGrip_T1");
		AddSlotTier("DF_handguard", "Handguard_T1");
		AddSlotTier("DF_handguard_AK12", "Handguard_T1");
		AddSlotTier("DF_handguard_AKM", "Handguard_T1");
		AddSlotTier("DF_handguard_AKS74U", "Handguard_T1");
		AddSlotTier("DF_handguard_AR", "Handguard_T1");
		AddSlotTier("DF_handguard_Ash12", "Handguard_T1");
		AddSlotTier("DF_handguard_AUG", "Handguard_T1");
		AddSlotTier("DF_handguard_AWM", "Handguard_T1");
		AddSlotTier("DF_handguard_Bison", "Handguard_T1");
		AddSlotTier("DF_handguard_Citori725", "Handguard_T1");
		AddSlotTier("DF_handguard_FS12", "Handguard_T1");
		AddSlotTier("DF_handguard_G3", "Handguard_T1");
		AddSlotTier("DF_handguard_K416", "Handguard_T1");
		AddSlotTier("DF_handguard_K437", "Handguard_T1");
		AddSlotTier("DF_handguard_KC17", "Handguard_T1");
		AddSlotTier("DF_handguard_M1014", "Handguard_T1");
		AddSlotTier("DF_handguard_M14", "Handguard_T1");
		AddSlotTier("DF_handguard_M249", "Handguard_T1");
		AddSlotTier("DF_handguard_M7", "Handguard_T1");
		AddSlotTier("DF_handguard_M700", "Handguard_T1");
		AddSlotTier("DF_handguard_M870", "Handguard_T1");
		AddSlotTier("DF_handguard_Marlin", "Handguard_T1");
		AddSlotTier("DF_handguard_MCXLT", "Handguard_T1");
		AddSlotTier("DF_handguard_Mini14", "Handguard_T1");
		AddSlotTier("DF_handguard_MK4", "Handguard_T1");
		AddSlotTier("DF_handguard_MK47", "Handguard_T1");
		AddSlotTier("DF_handguard_MP5", "Handguard_T1");
		AddSlotTier("DF_handguard_MP7", "Handguard_T1");
		AddSlotTier("DF_handguard_P90", "Handguard_T1");
		AddSlotTier("DF_handguard_PKM", "Handguard_T1");
		AddSlotTier("DF_handguard_PP19", "Handguard_T1");
		AddSlotTier("DF_handguard_QBZ191", "Handguard_T1");
		AddSlotTier("DF_handguard_QBZ95", "Handguard_T1");
		AddSlotTier("DF_handguard_QCQ171", "Handguard_T1");
		AddSlotTier("DF_handguard_QJB201", "Handguard_T1");
		AddSlotTier("DF_handguard_R93", "Handguard_T1");
		AddSlotTier("DF_handguard_S12K", "Handguard_T1");
		AddSlotTier("DF_handguard_SCARH", "Handguard_T1");
		AddSlotTier("DF_handguard_SG553", "Handguard_T1");
		AddSlotTier("DF_handguard_SKS", "Handguard_T1");
		AddSlotTier("DF_handguard_SMG45", "Handguard_T1");
		AddSlotTier("DF_handguard_SR25", "Handguard_T1");
		AddSlotTier("DF_handguard_SR3M", "Handguard_T1");
		AddSlotTier("DF_handguard_SVD", "Handguard_T1");
		AddSlotTier("DF_handguard_up_AKS74U", "Handguard_T1");
		AddSlotTier("DF_handguard_Uzi", "Handguard_T1");
		AddSlotTier("DF_handguard_Vector", "Handguard_T1");
		AddSlotTier("DF_Handle_PKM", "Handguard_T1");
		AddSlotTier("DF_Handle_QBZ95", "Handguard_T1");
		AddSlotTier("DF_mag_additional_Bison", "StandardMag_Neutral");
		AddSlotTier("DF_magwell_FS12", "StandardMag_Neutral");
		AddSlotTier("DF_mount", "Handguard_T1");
		AddSlotTier("DF_mount_G3", "Handguard_T1");
		AddSlotTier("DF_mount_M14", "Handguard_T1");
		AddSlotTier("DF_mount_M1911", "Handguard_T1");
		AddSlotTier("DF_mount_Pistol", "Handguard_T1");
		AddSlotTier("DF_mount_S12K", "Handguard_T1");
		AddSlotTier("DF_mount_SKS", "Handguard_T1");
		AddSlotTier("DF_mount_SR3M", "Handguard_T1");
		AddSlotTier("DF_mount_SVD", "Handguard_T1");
		AddSlotTier("DF_muzzle", "Muzzle_T1");
		AddSlotTier("DF_muzzle_Ash12", "Muzzle_T1");
		AddSlotTier("DF_muzzle_AWM", "Muzzle_T1");
		AddSlotTier("DF_muzzle_Citori725", "Muzzle_T1");
		AddSlotTier("DF_muzzle_Hide", "Muzzle_T1");
		AddSlotTier("DF_muzzle_Hive", "Muzzle_T1");
		AddSlotTier("DF_muzzlePistol", "Muzzle_T1");
		AddSlotTier("DF_muzzleShotgun", "Muzzle_T1");
		AddSlotTier("DF_pisg", "PistolGrip_T1");
		AddSlotTier("DF_pisg_93R", "PistolGrip_T1");
		AddSlotTier("DF_pisg_AKHouger", "PistolGrip_T1");
		AddSlotTier("DF_pisg_ColtPython", "PistolGrip_T1");
		AddSlotTier("DF_pisg_deagle", "PistolGrip_T1");
		AddSlotTier("DF_pisg_G3", "PistolGrip_T1");
		AddSlotTier("DF_pisg_Glock", "PistolGrip_T1");
		AddSlotTier("DF_pisg_M1911", "PistolGrip_T1");
		AddSlotTier("DF_pisg_M249", "PistolGrip_T1");
		AddSlotTier("DF_pisg_MP7", "PistolGrip_T1");
		AddSlotTier("DF_Piston_QBZ191", "Handguard_T1");
		AddSlotTier("DF_Piston_QJB201", "Handguard_T1");
		AddSlotTier("DF_PLA_pisg", "PistolGrip_T1");
		AddSlotTier("DF_quickdraw", "PistolGrip_T1");
		AddSlotTier("DF_R93HeatShield", "Handguard_T1");
		AddSlotTier("DF_SilencerCover_MCXLT", "Muzzle_T1");
		AddSlotTier("DF_StickLoaderLifter", "Handguard_T1");
		AddSlotTier("DF_stock", "Stock_T2");
		AddSlotTier("DF_stock_ACR_BodyPad", "Stock_T2");
		AddSlotTier("DF_stock_Ash12", "Stock_T2");
		AddSlotTier("DF_stock_Citori725", "Stock_T2");
		AddSlotTier("DF_stock_M700_A6", "Stock_T2");
		AddSlotTier("DF_stock_Marlin", "Stock_T2");
		AddSlotTier("DF_stock_notube", "Stock_T2");
		AddSlotTier("DF_stock_pad", "Stock_T2");
		AddSlotTier("DF_stock_pad_AUG", "Stock_T2");
		AddSlotTier("DF_stock_pad_P90", "Stock_T2");
		AddSlotTier("DF_stock_Revolver", "Stock_T2");
		AddSlotTier("DF_stock_special", "Stock_T2");
		AddSlotTier("DF_stock_SVD", "Stock_T2");
		AddSlotTier("DF_stock_VSS", "Stock_T2");
		AddSlotTier("DF_trigger_Citori725", "PistolGrip_T1");
		AddSlotTier("DF_trigger_M1911", "PistolGrip_T1");
		AddSlotTier("DF_trigger_Marlin", "PistolGrip_T1");
		AddSlotTier("DF_trigger_PSG", "PistolGrip_T1");
		AddSlotTier("Expansion_M1AScopeRail", "Handguard_T1");
		AddSlotTier("Expansion_MP5ScopeRail", "Handguard_T1");
		AddSlotTier("ExpansionKar98Optics", "SniperOptic_T3");
		AddSlotTier("ExpansionSniperOptics", "SniperOptic_T3");
		AddSlotTier("expansionWeaponBayonetKar", "Bayonet_Heavy");
		AddSlotTier("FGRIP", "Foregrip_T1");
		AddSlotTier("G3Optic", "HeavyOptic_T2");
		AddSlotTier("helmetFlashlight", "Flashlight_Utility");
		AddSlotTier("Knife", "Bayonet_Heavy");
		AddSlotTier("magazine", "StandardMag_Neutral");
		AddSlotTier("MP153Forearm", "Handguard_T1");
		AddSlotTier("pistolFlashlight", "Flashlight_Utility");
		AddSlotTier("PistolLaserPointer", "Laser_Tactical");
		AddSlotTier("pistolMuzzle", "Suppressor_T1");
		AddSlotTier("pistolOptics", "LightOptic_T1");
		AddSlotTier("suppressorImpro", "Suppressor_T1");
		AddSlotTier("SVT40_Optic", "HeavyOptic_T2");
		AddSlotTier("weaponAKPistolGrip", "PistolGrip_T2");
		AddSlotTier("weaponBayonet", "Bayonet_Heavy");
		AddSlotTier("weaponBayonetAK", "Bayonet_Heavy");
		AddSlotTier("weaponBayonetSKS", "Bayonet_Heavy");
		AddSlotTier("weaponBipod", "Bipod_Special");
		AddSlotTier("weaponButtstock", "Stock_T1");
		AddSlotTier("weaponButtstockADAR", "Stock_T2");
		AddSlotTier("weaponButtstockAK", "Stock_T2");
		AddSlotTier("weaponButtstockCQR", "Stock_T3");
		AddSlotTier("weaponButtstockFal", "Stock_T3");
		AddSlotTier("weaponButtstockG36", "Stock_T2");
		AddSlotTier("weaponButtstockM4", "Stock_T2");
		AddSlotTier("weaponButtstockMPXMCX", "Stock_T2");
		AddSlotTier("weaponevobuttstock", "Stock_T2");
		AddSlotTier("weaponevopistolgrip", "PistolGrip_T2");
		AddSlotTier("weaponFALOpticMount", "LightOptic_T1");
		AddSlotTier("weaponFALPistolGrip", "PistolGrip_T2");
		AddSlotTier("weaponFlashlight", "Flashlight_Utility");
		AddSlotTier("weaponFlashlightRail", "Flashlight_Utility");
		AddSlotTier("weaponForearmMP5", "Handguard_T1");
		AddSlotTier("weaponForearmMP5k", "Handguard_T1");
		AddSlotTier("weaponForegrip", "Foregrip_T1");
		AddSlotTier("weaponGripCQR", "Foregrip_T2");
		AddSlotTier("weaponGripKAAR", "Foregrip_T3");
		AddSlotTier("weaponHandguard", "Handguard_T1");
		AddSlotTier("weaponHandguardAK", "Handguard_T2");
		AddSlotTier("weaponHandguardASVAL", "Handguard_T2");
		AddSlotTier("weaponHandguardFAL", "Handguard_T2");
		AddSlotTier("weaponHandguardHK", "Handguard_T2");
		AddSlotTier("weaponHandguardHKG28", "Handguard_T2");
		AddSlotTier("weaponHandguardM16", "Handguard_T2");
		AddSlotTier("weaponHandguardM4", "Handguard_T2");
		AddSlotTier("weaponHandguardPK", "Handguard_T2");
		AddSlotTier("weaponHGAdarNA", "Handguard_T1");
		AddSlotTier("weaponLaser", "Laser_Tactical");
		AddSlotTier("weaponLaserPointer", "Laser_Tactical");
		AddSlotTier("weaponLightLeft", "Flashlight_Utility");
		AddSlotTier("weaponLightRight", "Flashlight_Utility");
		AddSlotTier("weaponLightTop", "Flashlight_Utility");
		AddSlotTier("weaponM4PistolGrip", "PistolGrip_T2");
		AddSlotTier("weaponMagazine", "StandardMag_Neutral");
		AddSlotTier("weaponMP5Buttstock", "Stock_T2");
		AddSlotTier("weaponMP5OpticMount", "LightOptic_T1");
		AddSlotTier("weaponMuzzle", "Muzzle_T1");
		AddSlotTier("weaponMuzzle277", "Muzzle_T1");
		AddSlotTier("weaponMuzzle308", "Muzzle_T1");
		AddSlotTier("weaponMuzzle338", "Muzzle_T1");
		AddSlotTier("weaponMuzzle408", "Muzzle_T1");
		AddSlotTier("weaponMuzzle86", "Muzzle_T1");
		AddSlotTier("weaponMuzzleAK", "Muzzle_T1");
		AddSlotTier("weaponMuzzleFAL", "Muzzle_T1");
		AddSlotTier("weaponMuzzleM4", "Muzzle_T1");
		AddSlotTier("weaponMuzzleMosin", "Muzzle_T1");
		AddSlotTier("weaponMuzzleMP5", "Muzzle_T1");
		AddSlotTier("weaponMuzzleShotgun", "Muzzle_T1");
		AddSlotTier("weaponMuzzleSKS", "Muzzle_T1");
		AddSlotTier("weaponOpticMount", "LightOptic_T1");
		AddSlotTier("weaponOpticMountAK", "LightOptic_T1");
		AddSlotTier("weaponOpticMountM16", "LightOptic_T1");
		AddSlotTier("weaponOptics", "LightOptic_T1");
		AddSlotTier("weaponOpticsAK", "LightOptic_T1");
		AddSlotTier("weaponOpticsCrossbow", "LightOptic_T1");
		AddSlotTier("weaponOpticsHunting", "SniperOptic_T3");
		AddSlotTier("weaponOpticsMosin", "SniperOptic_T3");
		AddSlotTier("weaponOpticsSniper", "SniperOptic_T3");
		AddSlotTier("weaponPKButtstock", "Stock_T2");
		AddSlotTier("weaponPKOpticMount", "LightOptic_T1");
		AddSlotTier("weaponScarButtstock", "Stock_T2");
		AddSlotTier("weaponSKSOpticMount", "LightOptic_T1");
		AddSlotTier("weaponSpas12Buttstock", "Stock_T2");
		AddSlotTier("weaponTubeStockAdapterAK", "Stock_T2");
		AddSlotTier("weaponTubeStockAdapterASVAL", "Stock_T2");
		AddSlotTier("weaponTubeStockAdapterScar", "Stock_T2");
		AddSlotTier("weaponTubeStockAdapterSVD", "Stock_T2");
		AddSlotTier("weaponTubeStockAdapterVector", "Stock_T2");
		AddSlotTier("weaponump45buttstock", "Stock_T2");
		AddSlotTier("weaponVectorButtstock", "Stock_T2");
		AddSlotTier("weaponWrap", "WeaponWrap_Utility");
	}
	
	protected static void AddSlotTier(string slotName, string tierKey)
	{
		m_SlotTierFallbacks.Set(slotName, tierKey);
	}
}
