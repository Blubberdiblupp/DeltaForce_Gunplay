modded class MagazineStorage : Magazine
{
	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		DeltaForceGunplayConfig config = DeltaForceGunplayConfig.GetInstance();
		if (config && !config.EnableWeaponGeometryDamage)
		{
			if (damageType == DamageType.FIRE_ARM)
			{
				if (config.DebugMode)
				{
					Print("[DeltaForce Gunplay] Magazine firearm damage blocked");
				}
				return false;
			}
		}

		return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}
}
