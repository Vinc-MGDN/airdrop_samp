#if defined _config_included
	#endinput
#endif

#define _config_included

#define function%0(%1)					forward %0(%1); public %0(%1)

#define AIRDROP_STATUS_INVALID			0
#define AIRDROP_STATUS_LAUNCH			1
#define AIRDROP_STATUS_START			2
#define AIRDROP_STATUS_CREATE			3
#define AIRDROP_STATUS_RUN				4
#define AIRDROP_STATUS_DROP				5
#define AIRDROP_STATUS_PICKEDUP			6

#define AIRDROP_INSTALLABLE_Z			50.0// âûñîòà ñàìîëåòà îò çåìëè
#define AIRPLANE_DISTANCE_DEPARTURE		400.0// ðàññòîÿíèå ñòàðòà ñàìîëåòà îò êðàÿ ãç
#define AIRPLANE_DISTANCE_ARRIVAL		800.0// ðàññòîÿíèå îñòàíîâêè ñàìîëåòà îò ìåñòà ñòàðòà(óäàëåíèå)
#define AIRPLANE_SPEED					25.0// ñêàîðîñòü äâèæåíèÿ ñàìîëåòà(äëÿ óâåë÷åíèÿ ñêîðîñòè, óâåëè÷èòü öèôðó)
#define PARACHUTE_SPEED					10.0// ñêîðîñòü ïàäåíèÿ ñóìêè ñ ïàðàøþòîì(äëÿ óâåë÷åíèÿ ñêîðîñòè, óâåëè÷èòü öèôðó)
#define START_AIRDROP_TIMER				(1000*60)// âðåìÿ, ÷åðåç êîòîðîå áóäåò çàïóùåíî ñîáûòèå. 1000*60 - ýòî 1 ìèíóòà. Äàëåå äåëàåì ðàíäîì íà ñâî¸ óñìîòðåíèå. Äàëåå ñìîòðèòå çàïóñê òàéìåðà â o_ad_ActionAirDrop(status), ñòàòóñ AIRDROP_STATUS_LAUNCH
#define AIRDROP_COLOR					0x02F89EFF
#define MAFIA_COLOR						0x8C8C8CFF
#define AIRDROP_FORMAT_COLOR			"{02F89E}"
#define MAFIA_FORMAT_COLOR				"{8C8C8C}"
#define AIRDROP_MAFIA_PRIZE_MONEY		200000
#define AIRDROP_MAFIA_PRIZE_RAITING		1500
#define AIRDROP_MAFIA_PRIZE_MATERIAL	25000
#define AIRDROP_PLAYER_PRIZE_MONEY		100000
#define AIRDROP_PLAYER_PRIZE_RAITING	1500
// --- ýòè äàííûé íå íóæíû, ò.ê. â êàæäîì ìîäå ñâîè ïåðåìåííûå, ñîäåðæàùèå äàííóþ èíôîðìàöèþ. Èñïîëüçóþòñÿ èñêëþ÷èòåëüíî äëÿ òåñòà ðàáîòû ñèñòåìû ---
#define MAX_MEMBER						4
// --- ïîäïðàâüòå ýòè ìàêðîñû ïîä ñåáÿ
#define GetPlayerOrg(%0)			GetPVarInt(%0,"ad_member")// âûàøè ïåðåìåííûå ñîäåðæàùèå äàííûé èãðîêà î ïðèíàäëåæíîñòè ê îðãàíèçàöèè
#define GivePlayerRating(%0,%1)		SetPVarInt(%0,"ad_rating",GetPVarInt(%0,"ad_rating")+%1)// âûàøè ïåðåìåííûå ñîäåðæàùèå äàííûé èãðîêà î åãî ðåéòèíãå
#define GivePlayerMoneyEx(%0,%1)	SetPVarInt(%0,"ad_money",GetPVarInt(%0,"ad_money")+%1)// âûàøè ïåðåìåííûå ñîäåðæàùèå äàííûé èãðîêà î åãî äåíüãàõ
#define GiveMemberMoney(%0,%1)		MI[%0][miMoney] += %1// âûàøè ïåðåìåííûå ñîäåðæàùèå äàííûé ôðàêöèè î äåíüãàõ
#define GiveMemberRating(%0,%1)		MI[%0][miRating] += %1// âûàøè ïåðåìåííûå ñîäåðæàùèå äàííûé ôðàêöèè î ðåéòèíãå
#define GiveMemberMaterial(%0,%1)	MI[%0][miMaterial] += %1// âûàøè ïåðåìåííûå ñîäåðæàùèå äàííûé ôðàêöèè î ìàòåðèàëàõ
// --- 
