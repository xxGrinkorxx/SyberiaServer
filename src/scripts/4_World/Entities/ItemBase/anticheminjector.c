modded class AntiChemInjector: Inventory_Base
{
     override void OnApply(PlayerBase player)
	{
        super.OnApply(player);
        player.ApplyAdvMedicineItem(GetType(), 1.0);
        PlayerBase operator = PlayerBase.Cast( GetHierarchyRootPlayer() ); 
        if (operator) operator.AddExperience(SyberiaSkillType.SYBSKILL_MEDICINE, GetSyberiaConfig().m_skillsExpMedicineInjector);
	}

};