#ifndef EU3COUNTRY_H_
#define EU3COUNTRY_H_



#include <string>
#include <vector>
#include "..\Date.h"
#include "..\Mappers.h"
#include "..\CK2World\CK2Title.h"
using namespace std;


class CK2Title;
class EU3Ruler;
class EU3Advisor;
class EU3History;
class EU3Province;
class EU3World;
class EU3Tech;
class CK2Province;

class EU3Country
{
	public:
		EU3Country(EU3World* world, string tag, string countryFile, date startDate, const EU3Tech* techData);
		EU3Country(CK2Title*, const religionMapping& religionMap, const cultureMapping& cultureMap, const inverseProvinceMapping& inverseProvinceMap);

		void		output(FILE*);
		void		determineLearningScore();
		void		addAcceptedCultures();
		void		determineGovernment(const religionGroupMapping& religionGroupMap);
		void		determineEconomy(const cultureGroupMapping& cultureGroups, const map<string, double>& unitPrices);
		double	getTradeEffeciency();
		double	getProductionEffeciency();
		void		determineTechLevels(const vector<double>& avgTechLevels, const EU3Tech* techData);
		void		determineTechInvestment(const EU3Tech* techData, date startDate);
		void		eatVassals();
		void		replaceWith(EU3Country* convertedCountry, const provinceMapping& provinceMappings);

		void		addLiege(EU3Country* _liege)			{ liege = _liege; _liege->addVassal(this); };
		void		addVassal(EU3Country* _vassal)		{ vassals.push_back(_vassal); };
		void		addProvince(EU3Province* province)	{ provinces.push_back(province); };
		void		addCore(EU3Province* core)				{ cores.push_back(core); };
		void		setTechGroup(string _techGroup)		{ techGroup = _techGroup; };
		void		setTag(string _tag)						{ tag = _tag; };
		void		addAdvisor(EU3Advisor* _advisor)		{ advisors.push_back(_advisor); };

		CK2Title*		getSrcCountry()			const { return src; };
		double			getLearningScore()		const { return learningScore; };
		string			getTag()						const { return tag; };
		string			getGovernment()			const { return government; };
		string			getPrimaryCulture()		const { return primaryCulture; };
		vector<string>	getAcceptedCultures()	const { return acceptedCultures; };
		string			getReligion()				const { return religion; };
		string			getTechGroup()				const { return techGroup; };
		int				getCapital()				const { return capital; };
		int				getStability()				const { return stability; };

	private:
		CK2Title*				src;
		EU3Country*				liege;
		vector<EU3Country*>	vassals;
		vector<EU3Province*>	provinces;
		vector<EU3Province*> cores;
		vector<EU3Advisor*>	advisors;
		double					learningScore;

		string					tag;
		string					historyFile;
		string					government;
		string					primaryCulture;
		vector<string>			acceptedCultures;
		string					religion;
		EU3Ruler*				monarch;
		EU3Ruler*				heir;
		EU3Ruler*				regent;
		vector<EU3History*>	history;
		vector<EU3Ruler*>		previousMonarchs;
		string					techGroup;
		double					governmentTech;
		double					productionTech;
		double					tradeTech;
		double					navalTech;
		double					landTech;
		double					governmentTechInvestment;
		double					productionTechInvestment;
		double					tradeTechInvestment;
		double					navalTechInvestment;
		double					landTechInvestment;

		int						capital;
		int						stability;
		double					stabilityInvestment;

		double					estimatedIncome;
		double					estimatedTax;
		double					estimatedGold;
		double					estimatedProduction;
		double					estimatedTolls;

		bool						daimyo;
		bool						japaneseEmperor;

		string					infantry;
		string					cavalry;
		string					bigShip;
		string					galley;
		string					transport;
};



#endif	// EU3COUNTRY_H_