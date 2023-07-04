modded class ActionCraftBoneKnifeEnv: ActionContinuousBase
{
override void OnFinishProgressServer( ActionData action_data )
	{
		
		super.OnFinishProgressServer(action_data);
        action_data.m_Player.AddExperienceOnCraftTool();
	}
};