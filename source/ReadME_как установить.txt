Краткое описание с технической стороны:
Система работает динамично. Чтобы добавить новые места дропа, Вам всего лишь стоит в массив fpc[][fall_position_coord] добавить требуемые данные.
Скорость самолета и парашюта, время начала самого аирдропа высоту и прочее - Все это можно настроить в макросах(инклуд config.inc).

Описание:

Для начала. В системе используется не стандартная библиотека streamer by Incognito. Не забываем его подключить.

Добавьте в папку с сервером инлуды системы airdop, примерный путь: ПАПКА_ВАШЕГО_СЕРВЕРА/source/org

В папку org закиньте инклуды config.inc | o_array.inc | o_airdrop.inc

В САМОЕ начало Вашего игрового мода добавьте строчки
#include "../source/config.inc"
#include "../source/o_array.inc"
#include "../source/o_airdrop.inc"

Далее в Вашем моде в следующие паблики(В САМОМ МОДЕ) вставьте:
public OnGameModeInit()
{
	o_ad_OnGameModeInit();
	// далее ваш код
	
public OnPlayerSpawn(playerid)
{
	o_ad_OnPlayerSpawn(playerid);
	// далее ваш код
	
public OnPlayerDeath(playerid, killerid, reason)
{
	o_ad_OnPlayerDeath(playerid,killerid,reason);
	// далее ваш код

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(o_ad_OnPlayerKeyStateChange(playerid,newkeys,oldkeys))return true;
	// далее ваш код
	
public OnPlayerPickUpDynamicPickup(playerid,pickupid)
{
	if(o_ad_OnPlayerPickUpDyn(playerid,pickupid))return true;
	// далее ваш код
	
public OnPlayerEnterDynamicCP(playerid, checkpointid)
{
	if(o_ad_OnPlayerEnterDynamicCP(playerid,checkpointid))return true;
	// далее ваш код

public OnDynamicObjectMoved(objectid)
{
	o_ad_OnDynamicObjectMoved(objectid);
	// далее ваш код
	
// Для теста можете добавить в
public OnPlayerText(playerid, text[])
{
	o_ad_OnPlayerText(playerid,text);
	// далее ваш код
	