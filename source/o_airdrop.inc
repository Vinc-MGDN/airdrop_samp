#if defined _o_airdrop_included
	#endinput
#endif

#define _o_airdrop_included

stock o_ad_OnGameModeInit()
{
	AD[adStatus] = AIRDROP_STATUS_INVALID;
	new test_mafia_name_1[] = "SRP_MAFIA";
	strmid(MI[1][miName],test_mafia_name_1,0,strlen(test_mafia_name_1),30);
	new test_mafia_name_2[] = "Русская Мафия";
	strmid(MI[2][miName],test_mafia_name_2,0,strlen(test_mafia_name_2),30);
	o_ad_ActionAirDrop(AIRDROP_STATUS_LAUNCH);
	SetSVarInt("pickedup_playerid",INVALID_PLAYER_ID);
	return true;
}
stock o_ad_OnPlayerSpawn(playerid)
{
	SetPVarInt(0,"ad_member",1);
	SetPVarInt(1,"ad_member",2);
	GetPlayerName(playerid,player_name[playerid],MAX_PLAYER_NAME);

	// ВСТАВЬТЕ ВМЕСТО PLAYER_NEW_SPAWN - ВАШУ ПРОВЕРКУ НА ТО, ЧТО ИГРОК БЫЛ ЗАСПАВНЕН ПОСЛЕ ВХОДА НА СЕРВЕР
	/*if(PLAYER_NEW_SPAWN && AD[adStatus] != AIRDROP_STATUS_INVALID)
	{
		switch(AD[adStatus])
		{
			case AIRDROP_STATUS_START,AIRDROP_STATUS_CREATE,AIRDROP_STATUS_RUN:
			{
				GangZoneShowForPlayer(playerid,AD[adGZid],fpc[random_zone][fpcColor]);
				SendClientMessage(playerid,MAFIA_COLOR,"Начато событие: "AIRDROP_FORMAT_COLOR"'Аирдроп'"MAFIA_FORMAT_COLOR", не пропустите! На Вашей карте отмечена метка!");
			}
			case AIRDROP_STATUS_DROP:SendClientMessage(playerid,MAFIA_COLOR,"Действует событие: "AIRDROP_FORMAT_COLOR"'Аирдроп'"MAFIA_FORMAT_COLOR", не пропустите! На Вашей карте отмечена метка!");
			case AIRDROP_STATUS_PICKEDUP:
			{
				if(GetSVarInt("pickedup_playerid") != INVALID_PLAYER_ID && IsPlayerConnected(GetSVarInt("pickedup_playerid")))
				{
					if(GetPlayerOrg(playerid) != GetPlayerOrg(GetSVarInt("pickedup_playerid")))
					{
						SendClientMessage(playerid,MAFIA_COLOR,"Действует событие: "AIRDROP_FORMAT_COLOR"'Аирдроп'"MAFIA_FORMAT_COLOR". Сумка поднята другой мафией!");
						SendClientMessage(playerid,MAFIA_COLOR,"Заберите сумку у члена другой мафии! На Вашей карте отмечена метка его местоположения!");
					}
					else 
					{
						SendClientMessage(playerid,MAFIA_COLOR,"Действует событие: "AIRDROP_FORMAT_COLOR"'Аирдроп'"MAFIA_FORMAT_COLOR". Сумка поднята Вашей мафией!");
						SendClientMessage(playerid,MAFIA_COLOR,"Защищайте члена свой мафии! На Вашей карте отмечена метка его местоположения!");
					}
				}
			}
		}
		o_ad_ShowMafiaADMapIcon();
	}*/
	return true;
}
stock o_ad_OnPlayerDeath(playerid,killerid,reason)
{
	#pragma unused killerid,reason
	if(o_ad_IsMemberMafia(GetPlayerOrg(playerid)) && AD[adStatus] != AIRDROP_STATUS_INVALID && GetPVarInt(playerid,"adTookBag"))
	{
		o_ad_UnpinBag(playerid);
		
		new Float:player_pos[3];
		GetPlayerPos(playerid,player_pos[0],player_pos[1],player_pos[2]);
		o_ad_BagFellOut(player_pos[0],player_pos[1],player_pos[2]);
		new str_form[89+30];
		if(killerid != INVALID_PLAYER_ID)format(str_form,sizeof(str_form),"%s потеряли сумку. Поймайте и отберите сумку у %s",MI[GetPlayerOrg(playerid)][miName],player_name[killerid]);
		else format(str_form,sizeof(str_form),"%s потеряли сумку. Подберите её!",MI[GetPlayerOrg(playerid)][miName]);
		for(new m; m < MAX_MEMBER; m++)
		{
			if(m == GetPlayerOrg(playerid))continue;
			o_ad_MafiaMessage(m,str_form);
		}
		format(str_form,sizeof(str_form),"%s потерял сумку. Верните её себе!",player_name[killerid]);
		o_ad_MafiaMessage(GetPlayerOrg(playerid),str_form);
	}
	return true;
}
stock o_ad_OnPlayerKeyStateChange(playerid,newkeys,oldkeys)
{
	#pragma unused oldkeys
	switch(newkeys)
	{
		case KEY_WALK:
		{
			if(o_ad_IsMemberMafia(GetPlayerOrg(playerid)) && !IsPlayerInAnyVehicle(playerid) && 
			IsValidDynamicObject(AD[adParachute_objectid]) && !IsDynamicObjectMoving(AD[adParachute_objectid]) && Streamer_IsItemVisible(playerid,STREAMER_TYPE_OBJECT,AD[adParachute_objectid]))
			{
				new Float:player_pos[3],Float:get_distantion_parachute;
				GetPlayerPos(playerid,player_pos[0],player_pos[1],player_pos[2]);
				Streamer_GetDistanceToItem(player_pos[0],player_pos[1],player_pos[2],STREAMER_TYPE_OBJECT,AD[adParachute_objectid],get_distantion_parachute,2);
				if(get_distantion_parachute <= 4.0)
				{
					DestroyDynamicObject(AD[adParachute_objectid]);
					DestroyDynamic3DTextLabel(AD[adBag_3Did]);
					o_ad_AttachedBag(playerid);

					GangZoneDestroy(AD[adGZid]);
					SetSVarInt("pickedup_playerid",playerid);
					AD[ad_timer] = SetTimerEx("o_ad_ActionAirDrop",1000*3,false,"i",AIRDROP_STATUS_PICKEDUP);
					return true;
				}
			}
		}
	}
	return false;
}
stock o_ad_OnPlayerPickUpDyn(playerid,pickupid)
{
	if(pickupid == AD[adBag_pickup])
	{
		DestroyDynamicPickup(pickupid);
		o_ad_AttachedBag(playerid);
		return true;
	}
	return false;
}
stock o_ad_OnPlayerEnterDynamicCP(playerid,checkpointid)
{
	if(checkpointid == AD[adBag_checkpoint])
	{
		if(o_ad_IsMemberMafia(GetPlayerOrg(playerid)) && AD[adStatus] != AIRDROP_STATUS_INVALID && GetPVarInt(playerid,"adTookBag"))
		{
			new str_form[104+30];
			format(str_form,sizeof(str_form),"Завершено событие: "AIRDROP_FORMAT_COLOR"'Аирдроп'"MAFIA_FORMAT_COLOR". Груз был доставлен фракцией %s",MI[GetPlayerOrg(playerid)][miName]);
			o_ad_AllMafiaMessage(str_form);
			
			GiveMemberMoney(GetPlayerOrg(playerid),AIRDROP_MAFIA_PRIZE_MONEY);
			GiveMemberRating(GetPlayerOrg(playerid),AIRDROP_MAFIA_PRIZE_RAITING);	
			GiveMemberMaterial(GetPlayerOrg(playerid),AIRDROP_MAFIA_PRIZE_MATERIAL);
			GivePlayerRating(playerid,AIRDROP_PLAYER_PRIZE_RAITING);
			GivePlayerMoneyEx(playerid,AIRDROP_PLAYER_PRIZE_MONEY);
			format(str_form,sizeof(str_form),"Ваша фракция получила: {33AA33}%i"MAFIA_FORMAT_COLOR"$ на банк мафии / {33AA33}%i"MAFIA_FORMAT_COLOR" рейтинга / {33AA33}%i"MAFIA_FORMAT_COLOR" материалов",AIRDROP_MAFIA_PRIZE_MONEY,AIRDROP_MAFIA_PRIZE_RAITING,AIRDROP_MAFIA_PRIZE_MATERIAL);
			o_ad_MafiaMessage(GetPlayerOrg(playerid),str_form);
			format(str_form,sizeof(str_form),"Игрок %s получил за доставку груза: {33AA33}%i"MAFIA_FORMAT_COLOR"$ / {33AA33}%i"MAFIA_FORMAT_COLOR" рейтинга",player_name[playerid],AIRDROP_PLAYER_PRIZE_MONEY,AIRDROP_PLAYER_PRIZE_RAITING);
			o_ad_MafiaMessage(GetPlayerOrg(playerid),str_form);

			o_ad_UnpinBag(playerid);
			o_ad_CloseEvent();
		}
		return true;
	}
	return false;
}
stock o_ad_OnDynamicObjectMoved(objectid)
{
	if(objectid == AD[adAirplane_objectid])
	{
		if(AD[adStatus] >= AIRDROP_STATUS_DROP)DestroyDynamicObject(objectid);
	}
	else if(objectid == AD[adParachute_objectid])
	{
		new Float:get_parachute_pos[3];
		GetDynamicObjectPos(objectid,get_parachute_pos[0],get_parachute_pos[1],get_parachute_pos[2]);
		AD[adBag_3Did] = CreateDynamic3DTextLabel("{FFFFFF}Сброшенная сумка\nНажмите {0D74FF}L.ALT{FFFFFF}, чтобы подобрать сумку",0xFFFFFFFF,get_parachute_pos[0],get_parachute_pos[1],get_parachute_pos[2]-6.0,10.0);
	}
	return true;
}
function o_ad_ActionAirDrop(status)
{
	switch(status)
	{
		
		case AIRDROP_STATUS_LAUNCH:// 
		{
			if(AD[adStatus] != AIRDROP_STATUS_INVALID)return print("[AIRDROP DEBUG] - сервер пытается запустить аирдроп, хотя он уже запущен.");
			// Выставляем рандом времени автозапуска события от 60 минут(1 час) до 120 минут(2 часа)
			AD[ad_timer] = SetTimerEx("o_ad_ActionAirDrop",o_ad_RandomEx(60,120)*START_AIRDROP_TIMER,false,"i",AIRDROP_STATUS_START);
		}
		case AIRDROP_STATUS_START:
		{
			if(AD[adStatus] == AIRDROP_STATUS_INVALID)return false;
			new random_zone = random(sizeof(fpc)),random_side = random(2),Float:airplane_create_pos[2],Float:airplane_direction_pos[2],Float:Get_angle,Float:start_airplane_pos[2],Float:finish_airplane_pos[2];
			switch(random_side)
			{
				case 0:
				{
					airplane_create_pos[0] = fpc[random_zone][fpcCoordXY][0],airplane_create_pos[1] = fpc[random_zone][fpcCoordXY][1];
					airplane_direction_pos[0] = fpc[random_zone][fpcCoordXY][2],airplane_direction_pos[1] = fpc[random_zone][fpcCoordXY][3];
				}
				default:
				{
					airplane_create_pos[0] = fpc[random_zone][fpcCoordXY][2],airplane_create_pos[1] = fpc[random_zone][fpcCoordXY][3];
					airplane_direction_pos[0] = fpc[random_zone][fpcCoordXY][0],airplane_direction_pos[1] = fpc[random_zone][fpcCoordXY][1];
				}
			}
			AD[adGZid] = GangZoneCreate(fpc[random_zone][fpcCoordXY][0],fpc[random_zone][fpcCoordXY][1],fpc[random_zone][fpcCoordXY][2],fpc[random_zone][fpcCoordXY][3]);

			new Float:get_center_gz[2];
			o_ad_GetCenterOfSquare(fpc[random_zone][fpcCoordXY][0],fpc[random_zone][fpcCoordXY][1],fpc[random_zone][fpcCoordXY][2],fpc[random_zone][fpcCoordXY][3],get_center_gz[0],get_center_gz[1]);

			new str_form[101+50];
			new district_id = GetDistrictCoord(get_center_gz[0],get_center_gz[1]);
			if(district_id != -1)format(str_form,sizeof(str_form),"Начато событие: "AIRDROP_FORMAT_COLOR"'Аирдроп'"MAFIA_FORMAT_COLOR". Груз будет сброшен в районе %s",MZN[district_id][mznName]);
			else format(str_form,sizeof(str_form),"Начато событие: "AIRDROP_FORMAT_COLOR"'Аирдроп'"MAFIA_FORMAT_COLOR".");
			o_ad_AllMafiaMessage(str_form);
			format(str_form,sizeof(str_form),"Груз будет выброшен через {33AA33}10 "MAFIA_FORMAT_COLOR"минут. Установлена метка на карте.");
			o_ad_AllMafiaMessage(str_form);

			// для теста телепортируем игрока под id 0 в центр ганг зоны
			SetPlayerPos(0,get_center_gz[0],get_center_gz[1],fpc[random_zone][fpcCoordZ]);
			// ---
			SetSVarFloat("ad_mapiconpos_x",get_center_gz[0]);
			SetSVarFloat("ad_mapiconpos_y",get_center_gz[1]);
			SetSVarFloat("ad_mapiconpos_z",fpc[random_zone][fpcCoordZ]);

			for(new p; p < GetMaxPlayers(); p++)
			{
				if(!IsPlayerConnected(p) || !o_ad_IsMemberMafia(GetPlayerOrg(p)))continue;
				GangZoneShowForPlayer(p,AD[adGZid],fpc[random_zone][fpcColor]);
			}
			o_ad_ShowMafiaADMapIcon();

			AD[adAirplane_zone] = random_zone;

			o_ad_GetRotationPostoPos(airplane_create_pos[0],airplane_create_pos[1],airplane_direction_pos[0],airplane_direction_pos[1],Get_angle);
			AD[adAirplane_objectid] = CreateDynamicObject(10757,airplane_create_pos[0],airplane_create_pos[1],fpc[random_zone][fpcCoordZ]+AIRDROP_INSTALLABLE_Z,0.0,0.0,Get_angle+180,123);

			o_ad_GetCoordsBefore(airplane_create_pos[0],airplane_create_pos[1],Get_angle,AIRPLANE_DISTANCE_DEPARTURE,start_airplane_pos[0],start_airplane_pos[1],2);
			new Float:get_object_z;
			Streamer_GetFloatData(STREAMER_TYPE_OBJECT,AD[adAirplane_objectid],E_STREAMER_Z,get_object_z);
			SetDynamicObjectPos(AD[adAirplane_objectid],start_airplane_pos[0],start_airplane_pos[1],get_object_z);

			o_ad_GetCoordsBefore(start_airplane_pos[0],start_airplane_pos[1],Get_angle,AIRPLANE_DISTANCE_ARRIVAL,finish_airplane_pos[0],finish_airplane_pos[1],1);
			SetSVarFloat("airplane_finish_pos_x",finish_airplane_pos[0]);
			SetSVarFloat("airplane_finish_pos_y",finish_airplane_pos[1]);
			SetSVarFloat("airplane_finish_pos_z",get_object_z);

			AD[ad_timer] = SetTimerEx("o_ad_ActionAirDrop",1000*60*10,false,"i",AIRDROP_STATUS_CREATE);// 1000*60*10 - запуск времени начала полета самолета на 10 минут
		}
		case AIRDROP_STATUS_CREATE:
		{	
			if(AD[adStatus] == AIRDROP_STATUS_INVALID)return false;
			MoveDynamicObject(AD[adAirplane_objectid],GetSVarFloat("airplane_finish_pos_x"),GetSVarFloat("airplane_finish_pos_y"),GetSVarFloat("airplane_finish_pos_z"),AIRPLANE_SPEED);
			Streamer_SetIntData(STREAMER_TYPE_OBJECT,AD[adAirplane_objectid],E_STREAMER_WORLD_ID,-1);
			DeleteSVar("airplane_finish_pos_x");
			DeleteSVar("airplane_finish_pos_y");
			DeleteSVar("airplane_finish_pos_z");

			AD[ad_timer] = SetTimerEx("o_ad_ActionAirDrop",500,false,"i",AIRDROP_STATUS_RUN);
		}
		case AIRDROP_STATUS_RUN:
		{
			if(AD[adStatus] == AIRDROP_STATUS_INVALID)return false;
			new Float:get_center_gz[2],Float:current_position[2];
			Streamer_GetFloatData(STREAMER_TYPE_OBJECT,AD[adAirplane_objectid],E_STREAMER_X,current_position[0]);
			Streamer_GetFloatData(STREAMER_TYPE_OBJECT,AD[adAirplane_objectid],E_STREAMER_Y,current_position[1]);
			o_ad_GetCenterOfSquare(fpc[AD[adAirplane_zone]][fpcCoordXY][0],fpc[AD[adAirplane_zone]][fpcCoordXY][1],fpc[AD[adAirplane_zone]][fpcCoordXY][2],fpc[AD[adAirplane_zone]][fpcCoordXY][3],get_center_gz[0],get_center_gz[1]);
			if(o_ad_DistancePointToPoint(get_center_gz[0],get_center_gz[1],current_position[0],current_position[1]) > 6.0)AD[ad_timer] = SetTimerEx("o_ad_ActionAirDrop",250,false,"i",AIRDROP_STATUS_RUN);
			else AD[ad_timer] = SetTimerEx("o_ad_ActionAirDrop",500,false,"i",AIRDROP_STATUS_DROP);
		}
		case AIRDROP_STATUS_DROP:
		{
			if(AD[adStatus] == AIRDROP_STATUS_INVALID)return false;
			new Float:get_airplane_pos[3];
			GetDynamicObjectPos(AD[adAirplane_objectid],get_airplane_pos[0],get_airplane_pos[1],get_airplane_pos[2]);
			AD[adParachute_objectid] = CreateDynamicObject(18849,get_airplane_pos[0],get_airplane_pos[1],get_airplane_pos[2]-10.0,0.0,0.0,0.0);
			MoveDynamicObject(AD[adParachute_objectid],get_airplane_pos[0],get_airplane_pos[1],fpc[AD[adAirplane_zone]][fpcCoordZ]+6.1,PARACHUTE_SPEED);
			new str_form[102+50];
			new district_id = GetDistrictCoord(get_airplane_pos[0],get_airplane_pos[1]);
			if(district_id != -1)format(str_form,sizeof(str_form),"Действует событие: "AIRDROP_FORMAT_COLOR"'Аирдроп'"MAFIA_FORMAT_COLOR". Груз был сброшен в районе %s",MZN[district_id][mznName]);
			else format(str_form,sizeof(str_form),"Действует событие: "AIRDROP_FORMAT_COLOR"'Аирдроп'"MAFIA_FORMAT_COLOR".");
			o_ad_AllMafiaMessage(str_form);
			format(str_form,sizeof(str_form),"Подберите груз и доставьте к особняку мафии.");
			o_ad_AllMafiaMessage(str_form);

			AD[adBag_checkpoint] = CreateDynamicCP(Coord_Marker_TheMafia_Territory[0][0],Coord_Marker_TheMafia_Territory[0][1],Coord_Marker_TheMafia_Territory[0][2],4.0);
			for(new p; p < GetMaxPlayers(); p++)
			{
				if(!IsPlayerConnected(p))continue;
				Streamer_ToggleItem(p,STREAMER_TYPE_CP,AD[adBag_checkpoint],0);
			}
		}
		case AIRDROP_STATUS_PICKEDUP:
		{
			if(AD[adStatus] == AIRDROP_STATUS_INVALID)return false;
			if(GetSVarInt("pickedup_playerid") != INVALID_PLAYER_ID && IsPlayerConnected(GetSVarInt("pickedup_playerid")))o_ad_ShowMafiaADMapIcon(GetSVarInt("pickedup_playerid")),AD[ad_timer] = SetTimerEx("o_ad_ActionAirDrop",1000*3,false,"i",AIRDROP_STATUS_PICKEDUP);
			else 
			{
				if(GetSVarInt("pickedup_playerid") != INVALID_PLAYER_ID && !IsPlayerConnected(GetSVarInt("pickedup_playerid")))o_ad_BagFellOut(GetSVarFloat("ad_mapiconpos_x"),GetSVarFloat("ad_mapiconpos_y"),GetSVarFloat("ad_mapiconpos_z"));
				o_ad_ShowMafiaADMapIcon();
				AD[ad_timer] = SetTimerEx("o_ad_ActionAirDrop",1000*3,false,"i",AIRDROP_STATUS_PICKEDUP);
			}
		}
	}
	AD[adStatus] = status;
	return true;
}
stock o_ad_BagFellOut(Float:pos_x,Float:pos_y,Float:pos_z)
{
	if(IsValidDynamicMapIcon(AD[adBag_pickup]))DestroyDynamicMapIcon(AD[adBag_pickup]);
	SetSVarInt("pickedup_playerid",INVALID_PLAYER_ID);
	AD[adBag_pickup] = CreateDynamicPickup(2919,23,pos_x,pos_y,pos_z);
	return true;
}
stock o_ad_IsMemberMafia(memberid)
{
	switch(memberid)
	{
		case 1..4:return true;// вместо 1..4 ваши id мафий
	}
	return false;
}
stock o_ad_AttachedBag(playerid)
{
	SetPlayerAttachedObject(playerid,5,2919,5,0.303000,0.032000,0.025999,-37.899971,-91.399948,2.699997,0.235000,0.192000,0.431000);
	SetPVarInt(playerid,"adTookBag",1);
	o_ad_ShowPlayerCheckpoint(playerid);
	SetSVarInt("pickedup_playerid",playerid);
	RemovePlayerMapIcon(playerid,31);
	SetPlayerMapIcon(playerid,31,Coord_Marker_TheMafia_Territory[GetPlayerOrg(playerid)][0],Coord_Marker_TheMafia_Territory[GetPlayerOrg(playerid)][1],Coord_Marker_TheMafia_Territory[GetPlayerOrg(playerid)][2],1,0x00FF00FF,MAPICON_GLOBAL);
	SendClientMessage(playerid,MAFIA_COLOR,"На Вашей карте установлена метка, отвезите туда груз!");

	new str_form[107+30];
	format(str_form,sizeof(str_form),"Действует событие: "AIRDROP_FORMAT_COLOR"'Аирдроп'"MAFIA_FORMAT_COLOR". Груз подобрала фракция %s",MI[GetPlayerOrg(playerid)][miName]);
	for(new m; m < MAX_MEMBER; m++)
	{
		if(m == GetPlayerOrg(playerid))continue;
		o_ad_MafiaMessage(m,str_form);
	}
	format(str_form,sizeof(str_form),"Поймайте и отберите сумку у %s",player_name[playerid]);
	for(new m; m < MAX_MEMBER; m++)
	{
		if(m == GetPlayerOrg(playerid))continue;
		o_ad_MafiaMessage(m,str_form);
	}
	format(str_form,sizeof(str_form),"Действует событие: "AIRDROP_FORMAT_COLOR"'Аирдроп'"MAFIA_FORMAT_COLOR". Член Вашей мафии %s подобрал груз",player_name[playerid]);
	o_ad_MafiaMessage(GetPlayerOrg(playerid),str_form);
	format(str_form,sizeof(str_form),"помогите ему доставить сумку на респавн мафии!");
	o_ad_MafiaMessage(GetPlayerOrg(playerid),str_form);

	return true;
}
stock o_ad_UnpinBag(playerid)
{
	RemovePlayerAttachedObject(playerid,5);
	DeletePVar(playerid,"adTookBag");
	Streamer_ToggleItem(playerid,STREAMER_TYPE_CP,AD[adBag_checkpoint],0);
	return true;
}
stock o_ad_ShowMafiaADMapIcon(update_coord_by_playerid = INVALID_PLAYER_ID)
{
	if(update_coord_by_playerid != INVALID_PLAYER_ID)
	{
		if(!IsPlayerConnected(update_coord_by_playerid) || !o_ad_IsMemberMafia(GetPlayerOrg(update_coord_by_playerid)))return false;
		new Float:player_pos[3];
		GetPlayerPos(update_coord_by_playerid,player_pos[0],player_pos[1],player_pos[2]);
		SetSVarFloat("ad_mapiconpos_x",player_pos[0]);
		SetSVarFloat("ad_mapiconpos_y",player_pos[1]);
		SetSVarFloat("ad_mapiconpos_z",player_pos[2]);
	}
	if(GetSVarFloat("ad_mapiconpos_last_x") == GetSVarFloat("ad_mapiconpos_x") && GetSVarFloat("ad_mapiconpos_last_y") == GetSVarFloat("ad_mapiconpos_y"))return false;
	for(new p; p < GetMaxPlayers(); p++)
	{
		if(GetSVarFloat("ad_mapiconpos_x") == 0.0 && GetSVarFloat("ad_mapiconpos_y") == 0.0)break;
		if(update_coord_by_playerid == p)continue;
		if(!IsPlayerConnected(p) || !o_ad_IsMemberMafia(GetPlayerOrg(p)))continue;
		SetPlayerMapIcon(p,31,GetSVarFloat("ad_mapiconpos_x"),GetSVarFloat("ad_mapiconpos_y"),GetSVarFloat("ad_mapiconpos_z"),37,0x00FF00FF,MAPICON_GLOBAL);
		SetPVarInt(p,"map_icon_fix_iconid",37);
	}
	return true;
}
stock UpdatePlayerMapIconFX(playerid)// В свази с тем, что samp не идеален, иконки иногда пропадают у игрока. Данной функцией мы фиксим эту проблему(иначе будут случаи, что игрок потеряет место дропа). Вставлять в Ваш таймер.
{
	if(GetPVarInt(playerid,"map_icon_fix_iconid") && GetPVarInt(playerid,"map_icon_fix_timer") < gettime())
	{
		SetPVarInt(playerid,"map_icon_fix_timer",gettime()+3);
		SetPlayerMapIcon(playerid,31,GetSVarFloat("ad_mapiconpos_x"),GetSVarFloat("ad_mapiconpos_y"),GetSVarFloat("ad_mapiconpos_z"),GetPVarInt(playerid,"map_icon_fix_iconid"),0x00FF00FF,MAPICON_GLOBAL);
	}
	return true;
}
stock o_ad_AllMafiaMessage(message[])
{
	for(new p; p < GetMaxPlayers(); p++)
	{
		if(!IsPlayerConnected(p) || !o_ad_IsMemberMafia(GetPlayerOrg(p)))continue;
		SendClientMessage(p,MAFIA_COLOR,message);
	}
	return true;
}
stock o_ad_MafiaMessage(mafiaid,message[])
{
	for(new p; p < GetMaxPlayers(); p++)
	{
		if(!IsPlayerConnected(p) || GetPlayerOrg(p) != mafiaid)continue;
		SendClientMessage(p,MAFIA_COLOR,message);
	}
	return true;
}
stock o_ad_ShowPlayerCheckpoint(playerid)
{
	Streamer_SetFloatData(STREAMER_TYPE_CP,AD[adBag_checkpoint],E_STREAMER_X,Coord_Marker_TheMafia_Territory[GetPlayerOrg(playerid)][0]);
	Streamer_SetFloatData(STREAMER_TYPE_CP,AD[adBag_checkpoint],E_STREAMER_Y,Coord_Marker_TheMafia_Territory[GetPlayerOrg(playerid)][1]);
	Streamer_SetFloatData(STREAMER_TYPE_CP,AD[adBag_checkpoint],E_STREAMER_Z,Coord_Marker_TheMafia_Territory[GetPlayerOrg(playerid)][2]);
	return Streamer_ToggleItem(playerid,STREAMER_TYPE_CP,AD[adBag_checkpoint],1);
}
stock o_ad_CloseEvent()
{
	KillTimer(AD[ad_timer]);
	AD[adStatus] = AIRDROP_STATUS_INVALID;
	SetSVarInt("pickedup_playerid",INVALID_PLAYER_ID);
	DeleteSVar("airplane_finish_pos_x");
	DeleteSVar("airplane_finish_pos_y");
	DeleteSVar("airplane_finish_pos_z");
	DeleteSVar("ad_mapiconpos_x");
	DeleteSVar("ad_mapiconpos_y");
	DeleteSVar("ad_mapiconpos_z");
	DeleteSVar("ad_mapiconpos_last_x");
	DeleteSVar("ad_mapiconpos_last_y");
	for(new p; p < GetMaxPlayers(); p++)
	{
		if(GetPVarInt(p,"adTookBag"))o_ad_UnpinBag(p);
		DeletePVar(p,"map_icon_fix_iconid");
		DeletePVar(p,"map_icon_fix_timer");
		DeletePVar(p,"adTookBag");
		RemovePlayerMapIcon(p,31);
	}
	if(IsValidDynamicObject(AD[adAirplane_objectid]))DestroyDynamicObject(AD[adAirplane_objectid]);
	GangZoneDestroy(AD[adGZid]);
	if(IsValidDynamicObject(AD[adParachute_objectid]))DestroyDynamicObject(AD[adParachute_objectid]);
	if(IsValidDynamicPickup(AD[adBag_pickup]))DestroyDynamicPickup(AD[adBag_pickup]);
	if(IsValidDynamic3DTextLabel(AD[adBag_3Did]))DestroyDynamic3DTextLabel(AD[adBag_3Did]);
	if(IsValidDynamicCP(AD[adBag_checkpoint]))DestroyDynamicCP(AD[adBag_checkpoint]);
	return true;
}
stock o_ad_RandomEx(min,max)
{
	return random(max - min + 1) + min;
}
stock GetDistrictCoord(Float:x,Float:y)
{
	for(new i;i < sizeof(MZN);i ++)
	{
	    if(	x >= MZN[i][mznCoord][0] && x <= MZN[i][mznCoord][3] && y >= MZN[i][mznCoord][1] && y <= MZN[i][mznCoord][4]) return i;
	}
	return -1;
}
stock o_ad_GetCenterOfSquare(Float:minx,Float:miny,Float:maxx,Float:maxy,&Float:getx,&Float:gety)
{
    getx = (minx + maxx) / 2.0;
    gety = (miny + maxy) / 2.0;
}  
stock o_ad_GetCoordsBefore(Float:x,Float:y,Float:a,Float:distance,&Float:res_x,&Float:res_y,type)
{
	switch(type)
	{
		case 1://координаты впереди
		{
			res_x = x + (distance * floatsin(-a, degrees));
			res_y = y + (distance * floatcos(-a, degrees));
		}
		case 2://координаты сзади
		{
			a = a + 180.0;
			res_x = x + (distance * floatsin(-a, degrees));
			res_y = y + (distance * floatcos(-a, degrees));
		}
	}
} 
stock o_ad_DistancePointToPoint(Float:x,Float:y,Float:fx,Float:fy) 
{	
	return floatround(floatsqroot(floatpower(fx - x, 2) + floatpower(fy - y, 2)));
}
stock Float:o_ad_CompressRotation(const Float:rotation)return (rotation-floatround(rotation/360.0,floatround_floor)*360.0);
stock o_ad_GetRotationPostoPos(const Float:x, const Float:y, const Float:tx, const Float:ty, &Float:rz)rz = o_ad_CompressRotation(atan2(ty-y,tx-x)-90.0);
// для тестов
stock o_ad_OnPlayerText(playerid,text[])
{
	if(!strcmp(text,"createairdrop",false))
	{
		o_ad_CloseEvent();
		AD[ad_timer] = SetTimerEx("o_ad_ActionAirDrop",500,false,"i",AIRDROP_STATUS_LAUNCH);
		SendClientMessage(playerid,0xFFFFFFFF,"аирдроп запущен");
	}
	return true;
}