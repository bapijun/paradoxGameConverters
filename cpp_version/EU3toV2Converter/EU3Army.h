#ifndef EU3ARMY_H_
#define EU3ARMY_H_


#include "stdafx.h"
#include "Parsers/Object.h"


typedef enum
{
	// ground units
	infantry = 0,
	cavalry,
	artillery,
	// navy units
	big_ship,
	light_ship,
	galley,
	transport,
	// end of enum
	num_reg_categories
} RegimentCategory;

typedef map<string, pair<RegimentCategory, int> > RegimentTypeMap;

class EU3Regiment // also Ship
{
	public:
		void					init(Object* obj);
		string					getName() const { return name; }
		string					getType() const { return type; }
		int						getHome() const { return home; }
		double					getStrength() const { return strength; }
		void					setCategory(RegimentCategory cat) { category = cat; }
		RegimentCategory		getCategory() const { return category; }
		void					setTypeStrength(int typeStrength) { type_strength = typeStrength; }
		int						getTypeStrength() const { return type_strength; }
	private:
		string					name;
		string					type;
		int						home;
		double					strength;
		RegimentCategory		category;
		int						type_strength;
};

class EU3Army // also Navy
{
	public:
		void					init(Object* obj);
		string					getName() const { return name; }
		int						getLocation() const { return location; }
		vector<EU3Regiment>		getRegiments() { return regiments; }
		void					resolveRegimentTypes(const RegimentTypeMap& regimentTypeMap);
		double					getAverageStrength(RegimentCategory category);
		int						getTotalTypeStrength(RegimentCategory category);
		int						getProbabilisticHomeProvince(RegimentCategory category);
		int						getAtSea() const { return at_sea; }
	private:
		string					name;
		int						location;
		int						at_sea;
		vector<EU3Regiment>		regiments;
};

#endif