modded class RecoilBase
{
	override vector GetRecoilModifier(Weapon_Base weapon)
	{
		if (weapon && weapon.DeltaForceShouldApplyGunplay() && weapon.IsBipodDeployed() && weapon.GetBipod().IsBipodValid())
		{
			float recoilModifier = weapon.GetDeltaForceWeaponRecoilModifier() * 0.2;
			return Vector(recoilModifier, recoilModifier, recoilModifier);
		}
		
		return super.GetRecoilModifier(weapon);
	}
}
