modded class ActionFishingNewCB : ActionContinuousBaseCB
{
override void  HandleFishingResultSuccess()
	{
        super.HandleFishingResultSuccess();
		if (!GetGame().IsMultiplayer() || GetGame().IsServer())
		{
            m_ActionDataFishing.m_Player.AddExperienceOnCatchFish();
        }
    }
};