#include "V2World.h"
#include <fstream>
#include <algorithm>
#include <io.h>
#include <list>
#include <math.h>
#include "Parsers/Parser.h"
#include "Log.h"
#include "LeaderTraits.h"
#include "tempFuncs.h"

#define MONEYFACTOR 30	// ducat to pound conversion rate

void V2World::init(string V2Loc)
{
	equalityLeft	= 6;
	libertyLeft		= 30;

	// set province names and numbers
	ifstream read;
	read.open( (V2Loc + "\\map\\definition.csv").c_str() );
	if (!read.is_open())
	{
		log("Error: Could not open \\map\\definition.csv\n");
		printf("Error: Could not open \\map\\definition.csv\n");
		read.close();
		exit(1);
	}

	string line;
	getline(read, line);
	while (!read.eof())
	{
		getline(read, line);
		unsigned int delimiter = line.find_first_of(';');
		if (delimiter == 0 || delimiter == string::npos)
		{
			continue;
		}
		int provNum = atoi( line.substr(0, delimiter).c_str() );

		V2Province newProv;
		newProv.init(provNum);
		provinces.push_back(newProv);
	}
	read.close();
	read.clear();

	// set province names
	read.open( (V2Loc + "\\localisation\\text.csv").c_str() );
	while (read.good() && !read.eof())
	{
		getline(read, line);
		if (line.substr(0,4) == "PROV" && isdigit(line.c_str()[4]))
		{
			int position = line.find_first_of(';');
			int num = atoi( line.substr(4, position - 4).c_str() );
			string name = line.substr(position + 1, line.find_first_of(';', position + 1) - position - 1);
			for (unsigned int i = 0; i < provinces.size(); i++)
			{
				if (provinces[i].getNum() == num)
				{
					provinces[i].setName(name);
					break;
				}
			}
		}
	}
	read.close();
	read.clear();
	read.open( (V2Loc + "\\localisation\\1.1.csv").c_str() );
	while (read.good() && !read.eof())
	{
		getline(read, line);
		if (line.substr(0,4) == "PROV" && isdigit(line.c_str()[4]))
		{
			int position = line.find_first_of(';');
			int num = atoi( line.substr(4, position - 4).c_str() );
			string name = line.substr(position + 1, line.find_first_of(';', position + 1) - position - 1);
			for (unsigned int i = 0; i < provinces.size(); i++)
			{
				if (provinces[i].getNum() == num)
				{
					provinces[i].setName(name);
					break;
				}
			}
		}
	}
	read.close();
	read.clear();
	read.open( (V2Loc + "\\localisation\\1.2.csv").c_str() );
	while (read.good() && !read.eof())
	{
		getline(read, line);
		if (line.substr(0,4) == "PROV" && isdigit(line.c_str()[4]))
		{
			int position = line.find_first_of(';');
			int num = atoi( line.substr(4, position - 4).c_str() );
			string name = line.substr(position + 1, line.find_first_of(';', position + 1) - position - 1);
			for (unsigned int i = 0; i < provinces.size(); i++)
			{
				if (provinces[i].getNum() == num)
				{
					provinces[i].setName(name);
					break;
				}
			}
		}
	}
	read.close();
	read.clear();
	read.open( (V2Loc + "\\localisation\\beta1.csv").c_str() );
	while (read.good() && !read.eof())
	{
		getline(read, line);
		if (line.substr(0,4) == "PROV" && isdigit(line.c_str()[4]))
		{
			int position = line.find_first_of(';');
			int num = atoi( line.substr(4, position - 4).c_str() );
			string name = line.substr(position + 1, line.find_first_of(';', position + 1) - position - 1);
			for (unsigned int i = 0; i < provinces.size(); i++)
			{
				if (provinces[i].getNum() == num)
				{
					provinces[i].setName(name);
					break;
				}
			}
		}
	}
	read.close();
	read.clear();
	read.open( (V2Loc + "\\localisation\\beta2.csv").c_str() );
	while (read.good() && !read.eof())
	{
		getline(read, line);
		if (line.substr(0,4) == "PROV" && isdigit(line.c_str()[4]))
		{
			int position = line.find_first_of(';');
			int num = atoi( line.substr(4, position - 4).c_str() );
			string name = line.substr(position + 1, line.find_first_of(';', position + 1) - position - 1);
			for (unsigned int i = 0; i < provinces.size(); i++)
			{
				if (provinces[i].getNum() == num)
				{
					provinces[i].setName(name);
					break;
				}
			}
		}
	}
	read.close();
	read.clear();

	// set province rgo types and life ratings
	struct _finddata_t	provDirData;
	intptr_t					fileListing;
	if ( (fileListing = _findfirst( (V2Loc + "\\history\\provinces\\*").c_str(), &provDirData)) == -1L)
	{
		log("Could not open province directories.\n");
		return;
	}
	do
	{
		string provDirPath = V2Loc + "\\history\\provinces\\" + provDirData.name + "\\";
		if (strcmp(provDirData.name, ".") == 0 || strcmp(provDirData.name, "..") == 0 )
		{
			continue;
		}
		struct _finddata_t	provFileData;
		intptr_t					fileListing2;
		if ( (fileListing2 = _findfirst( (provDirPath + "*").c_str(), &provFileData)) == -1L)
		{
			return;
		}
		do
		{
			if (strcmp(provFileData.name, ".") == 0 || strcmp(provFileData.name, "..") == 0 )
			{
				continue;
			}
			string filename(provFileData.name);
			int provNum = atoi( filename.substr(0, filename.find_first_of(' ')).c_str() );
			int delimiter = filename.find_last_of(' ');
			string provName = filename.substr(delimiter + 1, filename.find_first_of('.') - delimiter - 1);

			vector<V2Province>::iterator i;
			for(i = provinces.begin(); i != provinces.end(); i++)
			{
				if (i->getNum() == provNum)
				{
					read.open( (provDirPath + provFileData.name).c_str(), ios_base::binary );
					vector<string> lines;
					while (!read.eof())
					{
						getline(read, line);
						int delimiter2 = 0;
						do
						{
							int delimiter3 = line.find_first_of('\r', delimiter2);
							if (delimiter3 == line.size() - 1)
							{
								lines.push_back(line.substr(delimiter2, line.size() - delimiter2 - 1));
								delimiter2 = string::npos;
							}
							else if (delimiter3 == string::npos)
							{
								lines.push_back(line);
								delimiter2 = string::npos;
							}
							else
							{
								lines.push_back(line.substr(delimiter2, delimiter3 - delimiter2));
								delimiter2 = delimiter3 + 1;
							}
						} while (delimiter2 != string::npos);
					}
					for (unsigned int j = 0; j < lines.size(); j++)
					{
						if(lines[j].substr(0, 14) == "trade_goods = ")
						{
							string type = lines[j].substr(14, lines[j].size() - 14);
							i->setRgoType(type);
						}
						if(lines[j].substr(0, 14) == "life_rating = ")
						{
							int rating = atoi( lines[j].substr(14, lines[j].size() - 14).c_str() );
							i->setLifeRating(rating);
						}
					}
					read.close();
					read.clear();
					break;
				}
			}
			if (i == provinces.end())
			{
				log("	Error: could not find province %d to add rgo and liferating.\n", provNum);
			}


			//provinces.push_back(newProv);
		} while(_findnext(fileListing2, &provFileData) == 0);
		_findclose(fileListing2);
	} while(_findnext(fileListing, &provDirData) == 0);
	_findclose(fileListing);

	// set V2 basic population levels
	struct _finddata_t	popsFileData;
	if ( (fileListing = _findfirst( (V2Loc + "\\history\\pops\\1836.1.1\\*").c_str(), &popsFileData)) == -1L)
	{
		log("Could not open pops files.\n");
		return;
	}
	do
	{
		if (strcmp(popsFileData.name, ".") == 0 || strcmp(popsFileData.name, "..") == 0 )
		{
			continue;
		}

		Object*	obj2;				// generic object
		ifstream	read;				// ifstream for reading files
		initParser();
		obj2 = getTopLevel();
		read.open( (V2Loc + "\\history\\pops\\1836.1.1\\" + popsFileData.name).c_str() );
		if (!read.is_open())
		{
			log("Error: Could not open %s\n", popsFileData.name);
			printf("Error: Could not open %s\n", popsFileData.name);
			read.close();
			read.clear();
			continue;
		}
		readFile(read);
		read.close();
		read.clear();
	
		vector<Object*> leaves = obj2->getLeaves();
		for (unsigned int j = 0; j < leaves.size(); j++)
		{
			int provNum = atoi(leaves[j]->getKey().c_str());
			unsigned int k = 0;
			while (k < provinces.size() && provNum != provinces[k].getNum())
			{
				k++;
			}
			if (k == provinces.size())
			{
				log("Could not find province %d for original pops.\n", provNum);
				continue;
			}
			else
			{
				vector<Object*> pops = leaves[j]->getLeaves();
				for(unsigned int l = 0; l < pops.size(); l++)
				{
					V2Pop newPop;
					newPop.setType(pops[l]->getKey());
					newPop.setCulture(pops[l]->getLeaf("culture"));
					newPop.setReligion(pops[l]->getLeaf("religion"));
					newPop.setSize(atoi(pops[l]->getLeaf("size").c_str()));
					provinces[k].addOldPop(newPop);
				}
			}
		}
	} while(_findnext(fileListing, &popsFileData) == 0);
	_findclose(fileListing);

	// determine whether a province is coastal or not by checking if it has a naval base
	// if it's not coastal, we won't try to put any navies in it (otherwise Vicky crashes)
	Object*	obj2 = doParseFile((V2Loc + "\\map\\positions.txt").c_str());
	vector<Object*> objProv = obj2->getLeaves();
	if (objProv.size() == 0)
	{
		log("Error: map\\positions.txt failed to parse.");
		printf("Error: map\\positions.txt failed to parse.");
		exit(1);
	}
	for (vector<Object*>::iterator itr = objProv.begin(); itr != objProv.end(); ++itr)
	{
		int provinceNum = atoi((*itr)->getKey().c_str());
		vector<Object*> objPos = (*itr)->getValue("building_position");
		if (objPos.size() == 0)
			continue;
		vector<Object*> objNavalBase = objPos[0]->getValue("naval_base");
		if (objNavalBase.size() != 0)
		{
			// this province is coastal
			for (vector<V2Province>::iterator pitr = provinces.begin(); pitr != provinces.end(); ++pitr)
			{
				if (pitr->getNum() == provinceNum)
				{
					pitr->setCoastal(true);
					break;
				}
			}
		}
	}
}


void V2World::addPotentialCountries(ifstream &countriesMapping, string V2Loc)
{
	int partiesIndex = 1;
	while (!countriesMapping.eof())
	{
		string line;
		getline(countriesMapping, line);

		if ( (line[0] == '#') | (line.size() < 3) )
		{
			continue;
		}
		
		string tag;
		tag = line.substr(0, 3);

		string countryFileName;
		int start			= line.find_first_of('/');
		int size				= line.find_last_of('\"') - start;
		countryFileName	= line.substr(start, size);

		int oldPartiesIndex = partiesIndex;
		ifstream countryFile( (V2Loc + "\\common\\countries\\" + countryFileName).c_str());
		vector<int> parties;
		while (!countryFile.eof())
		{
			string line2;
			getline(countryFile, line2);
			if (line2 == "party = {")
			{
				getline(countryFile, line2);	// party name
				getline(countryFile, line2);	// start date
				int start2	= line2.find_first_of('=') + 2;
				int size2	= line2.size() - start2;
				line2 = line2.substr(start2, size2);

				int end = line2.find_first_of('.');
				line2 = line2.substr(0, end);  // now should be just the year

				if (atoi(line2.c_str()) <= 1836)
				{
					parties.push_back(partiesIndex);
				}

				partiesIndex++;
			}
		}

		if (tag == "REB")
		{
			continue;
		}
		V2Country newCountry;
		newCountry.init(tag, countryFileName, parties);
		potentialCountries.push_back(newCountry);
	}
}


void V2World::sortCountries(const vector<string>& order)
{
	vector<V2Country> sortedCountries;
	for (vector<string>::const_iterator oitr = order.begin(); oitr != order.end(); ++oitr)
	{
		for (vector<V2Country>::iterator itr = countries.begin(); itr != countries.end(); ++itr)
		{
			if (itr->getTag() == (*oitr))
			{
				sortedCountries.push_back(*itr);
				break;
			}
		}
	}
	countries.swap(sortedCountries);
}


vector<string> V2World::getPotentialTags()
{
	vector<string> tagList;
	for (unsigned int i = 0; i < potentialCountries.size(); i++)
	{
		tagList.push_back(potentialCountries[i].getTag());
	}
	return tagList;
}


void V2World::convertCountries(EU3World sourceWorld, countryMapping countryMap, cultureMapping cultureMap, religionMapping religionMap, governmentMapping governmentMap)
{
	vector<string> outputOrder = getPotentialTags();
	vector<EU3Country> sourceCountries = sourceWorld.getCountries();
	for (unsigned int i = 0; i < sourceCountries.size(); i++)
	{
		V2Country newCountry;
		countryMapping::iterator iter;
		iter = countryMap.find(sourceCountries[i].getTag());
		if (iter != countryMap.end())
		{
			for(unsigned int j = 0; j < potentialCountries.size(); j++)
			{
				if (potentialCountries[j].getTag() == iter->second.c_str())
				{
					newCountry = potentialCountries[j];
					newCountry.setSourceCountryIndex(i);

					if ( (sourceCountries[i].getTechGroup() == "western") || (sourceCountries[i].getTechGroup() == "eastern") || (sourceCountries[i].getTechGroup() == "ottoman"))
					{
						newCountry.setCivilized(true);
					}
					else
					{
						newCountry.setCivilized(false);
					}
					
					string srcCulture = sourceCountries[i].getPrimaryCulture();
					if (srcCulture.size() > 0)
					{
						bool matched = false;
						for (size_t k = 0; (k < cultureMap.size()) && (!matched); k++)
						{
							bool match = true;
							if (cultureMap[k].srcCulture == srcCulture)
							{
								for (size_t j = 0; j < cultureMap[k].distinguishers.size(); j++)
								{
									if (cultureMap[k].distinguishers[j].first == owner)
									{
										if (newCountry.getTag() != cultureMap[k].distinguishers[j].second)
											match = false;
									}
									else if (cultureMap[k].distinguishers[j].first == religion)
									{
										if (newCountry.getReligion() != cultureMap[k].distinguishers[j].second)
											match = false;
									}
									else
									{
										log ("Error: Unhandled distinguisher type in culture rules.\n");
									}
								}
								if (match)
								{
									newCountry.setPrimaryCulture(cultureMap[k].dstCulture);
									matched = true;
								}
							}
						}
						if (!matched)
						{
							log("No culture mapping defined for %s (%s -> %s)\n", srcCulture.c_str(), sourceCountries[i].getTag().c_str(), newCountry.getTag().c_str());
						}
					}
					
					vector<string> acceptedCultures = sourceCountries[i].getAcceptedCultures();
					for (unsigned int k = 0; k < acceptedCultures.size(); k++)
					{
						bool matched = false;
						for (size_t l = 0; (l < cultureMap.size()) && (!matched); l++)
						{
							bool match = true;
							if (cultureMap[l].srcCulture == acceptedCultures[k])
							{
								for (size_t j = 0; j < cultureMap[l].distinguishers.size(); j++)
								{
									if (cultureMap[l].distinguishers[j].first == owner)
									{
										if (newCountry.getTag() != cultureMap[l].distinguishers[j].second)
											match = false;
									}
									else if (cultureMap[l].distinguishers[j].first == religion)
									{
										if (newCountry.getReligion() != cultureMap[l].distinguishers[j].second)
											match = false;
									}
									else
									{
										log ("Error: Unhandled distinguisher type in culture rules.\n");
									}
								}
								if (match)
								{
									newCountry.addAcceptedCulture(cultureMap[l].dstCulture);
									matched = true;
								}
							}
						}
						if (!matched)
						{
							log("No culture mapping defined for %s (%s -> %s)\n", srcCulture.c_str(), sourceCountries[i].getTag().c_str(), newCountry.getTag().c_str());
						}
					}

					string srcReligion = sourceCountries[i].getReligion();
					if (srcReligion.size() > 0)
					{
						religionMapping::iterator iter2 = religionMap.find(srcReligion);
						if (iter2 != religionMap.end())
						{
							newCountry.setReligion(iter2->second);
						}
						else
						{
							log("Error: No religion mapping defined for %s (%s -> %s)\n", srcReligion.c_str(), sourceCountries[i].getTag().c_str(), newCountry.getTag().c_str());
						}
					}

					newCountry.setPrestige(sourceCountries[i].getPrestige() + 100);

					string srcGovernment = sourceCountries[i].getGovernment();
					if (srcGovernment.size() > 0)
					{
						governmentMapping::iterator iter2 = governmentMap.find(srcGovernment);
						if (iter2 != governmentMap.end())
						{
							newCountry.setGovernment(iter2->second);
						}
						else
						{
							log("Error: No government mapping defined for %s (%s -> %s)\n", srcGovernment.c_str(), sourceCountries[i].getTag().c_str(), newCountry.getTag().c_str());
						}
					}

					newCountry.setReforms(&sourceCountries[newCountry.getSourceCountryIndex()]);
					newCountry.setNationalIdea(&sourceCountries[newCountry.getSourceCountryIndex()], libertyLeft, equalityLeft);

					vector<EU3Relations> srcRelations = sourceCountries[i].getRelations();
					if (srcRelations.size() > 0)
					{
						for (vector<EU3Relations>::iterator itr = srcRelations.begin(); itr != srcRelations.end(); ++itr)
						{
							countryMapping::iterator newTag = countryMap.find(itr->getCountry());
							if (newTag != countryMap.end())
							{
								V2Relations v2r;
								v2r.init(newTag->second);
								v2r.setRelations(itr->getRelations());
								v2r.setMilitaryAccess(itr->hasMilitaryAccess());
								v2r.setDiplomatLastSent(itr->getDiplomatLastSent());
								v2r.setLastWar(itr->getLastWar());
								newCountry.addRelations(v2r);
							}
						}
					}
					newCountry.sortRelations(outputOrder);

					newCountry.setMoney(MONEYFACTOR * sourceCountries[i].inflationAdjust(sourceCountries[i].getTreasury()));
					newCountry.setLastBankrupt(sourceCountries[i].getLastBankrupt());

					vector<EU3Loan> srcLoans = sourceCountries[i].getLoans();
					for (vector<EU3Loan>::iterator itr = srcLoans.begin(); itr != srcLoans.end(); ++itr)
					{
						string lender = newCountry.getTag();
						if (itr->getLender() != "---")
						{
							countryMapping::iterator newTag = countryMap.find(itr->getLender());
							if (newTag != countryMap.end())
							{
								lender = newTag->second;
							}
							else
							{
								log("Error: lender %s could not be found for %s's loan!\n", itr->getLender().c_str(), newCountry.getTag().c_str());
							}
						}
						double size = MONEYFACTOR * sourceCountries[i].inflationAdjust(itr->getAmount());
						newCountry.addLoan(lender, size, itr->getInterest() / 100.0f);
					}

					newCountry.setDiploPoints(2.0 * sourceCountries[i].getDiplomats());

					newCountry.setBadboy((25.0 / sourceCountries[i].getBadboyLimit()) * sourceCountries[i].getBadboy());
				}
			}
		}
		else
		{
			log("Error: Could not convert EU3 tag %s to V2.\n", sourceCountries[i].getTag().c_str());
			printf("Error: Could not convert EU3 tag %s to V2.\n", sourceCountries[i].getTag().c_str());
			vector<int> empty;
			newCountry.init("", "", empty);
		}

		countries.push_back(newCountry);
	}

	// put countries in the same order as potentialCountries was (this is the same order V2 will save them in)
	sortCountries(outputOrder);
}


struct MTo1ProvinceComp
{
	MTo1ProvinceComp() : totalPopulation(0) {};

	vector<EU3Province*> provinces;
	int totalPopulation;
};

void V2World::convertProvinces(EU3World sourceWorld, provinceMapping provMap, countryMapping contMap, cultureMapping cultureMap, religionMapping religionMap)
{
	for (unsigned int i = 0; i < provinces.size(); i++)
	{
		int destNum					= provinces[i].getNum();
		vector<int> sourceNums	= provMap[destNum];
		if (sourceNums.size() == 0)
		{
			log("Error: no source for %s (province #%d)\n", provinces[i].getName().c_str() , destNum);
			//provinces[i].setOwner("\"ERR\"");
		}
		else
		{
			EU3Province* oldProvince = NULL;
			string oldOwner = "";
			// determine ownership by province count, or total population (if province count is tied)
			map<string, MTo1ProvinceComp> provinceBins;
			double newProvinceTotalPop = 0;
			for (vector<int>::iterator itr = sourceNums.begin(); itr != sourceNums.end(); ++itr)
			{
				EU3Province* province = sourceWorld.getProvince(*itr);
				if (!province)
				{
					log("Error: old province %d does not exist.  Bad mapping?\n", sourceNums[0]);
					continue;
				}
				string tag = province->getOwner();
				if (provinceBins.find(tag) == provinceBins.end())
					provinceBins[tag] = MTo1ProvinceComp();
				provinceBins[tag].provinces.push_back(province);
				provinceBins[tag].totalPopulation += province->getPopulation();
				newProvinceTotalPop += province->getPopulation();
				// I am the new owner if there is no current owner, or I have more provinces than the current owner,
				// or I have the same number of provinces, but more population, than the current owner
				if (   (oldOwner == "")
					|| (provinceBins[tag].provinces.size() > provinceBins[oldOwner].provinces.size())
					|| ((provinceBins[tag].provinces.size() == provinceBins[oldOwner].provinces.size())
					    && (provinceBins[tag].totalPopulation > provinceBins[oldOwner].totalPopulation)))
				{
					oldOwner = tag;
					oldProvince = province;
				}
			}

			if (oldOwner != "")
			{
				countryMapping::iterator iter = contMap.find(oldOwner);
				if (iter == contMap.end())
				{
					log("Error: Could not map provinces owned by %s.\n", oldOwner.c_str());
				}
				else
				{
					provinces[i].setOwner(iter->second);
					provinces[i].setColonial(oldProvince->isColony());
					provinces[i].setColonised(oldProvince->wasColonised());

					for (map<string, MTo1ProvinceComp>::iterator mitr = provinceBins.begin(); mitr != provinceBins.end(); ++mitr)
					{
						for (vector<EU3Province*>::iterator vitr = mitr->second.provinces.begin(); vitr != mitr->second.provinces.end(); ++vitr)
						{
							vector<string> oldCores = (*vitr)->getCores();
							for(unsigned int j = 0; j < oldCores.size(); j++)
							{
								// skip this core if the country is the owner of the EU3 province but not the V2 province
								// (i.e. "avoid boundary conflicts that didn't exist in EU3").
								// this country may still get core via a province that DID belong to the current V2 owner
								if ((oldCores[j] == mitr->first) && (oldCores[j] != oldOwner))
									continue;

								iter = contMap.find(oldCores[j]);
								if (iter != contMap.end())
								{
									provinces[i].addCore(iter->second);
								}
							}

							double provPopRatio = (*vitr)->getPopulation() / newProvinceTotalPop;
							(*vitr)->buildPopRatios();
							vector<EU3PopRatio> popRatios = (*vitr)->getPopRatios();
							for (vector<EU3PopRatio>::iterator pritr = popRatios.begin(); pritr != popRatios.end(); ++pritr)
							{
								bool matched = false;
								string culture = "";
								for (size_t k = 0; (k < cultureMap.size()) && (!matched); k++)
								{
									if (cultureMap[k].srcCulture == pritr->culture)
									{
										bool match = true;
										for (size_t j = 0; j < cultureMap[k].distinguishers.size(); j++)
										{
											if (cultureMap[k].distinguishers[j].first == owner)
											{
												if ((*vitr)->getOwner() != cultureMap[k].distinguishers[j].second)
													match = false;
											}
											else if (cultureMap[k].distinguishers[j].first == religion)
											{
												if (pritr->religion != cultureMap[k].distinguishers[j].second)
													match = false;
											}
											else
											{
												log ("Error: Unhandled distinguisher type in culture rules.\n");
											}

										}
										if (match)
										{
											culture = cultureMap[k].dstCulture;
											matched = true;
										}
									}
								}
								if (!matched)
								{
									log("Error: Could not set culture for pops in province %d\n", destNum);
								}

								string religion = "";
								religionMapping::iterator iter3 = religionMap.find(pritr->religion);
								if (iter3 != religionMap.end())
								{
									religion = iter3->second;
								}
								else
								{
									log("Error: Could not set religion for pops in province %d\n", destNum);
								}

								provinces[i].createPops(culture, religion, pritr->popRatio * provPopRatio, (*vitr), sourceWorld.getCountry(oldOwner));
							}
						}
					}
				}
			}
		}
		provinces[i].combinePops();
	}
}


void V2World::addUnions(unionMapping unionMap)
{
	for (unsigned int i = 0; i < provinces.size(); i++)
	{
		for (unsigned int j = 0; j < unionMap.size(); j++)
		{
			if ( (unionMap[j].first == provinces[i].getCulture()) && (!provinces[i].wasColonised()) )
			{
				provinces[i].addCore(unionMap[j].second);
			}
		}
	}
}


void V2World::convertCapitals(EU3World sourceWorld, provinceMapping provinceMap)
{
	inverseProvinceMapping inverseProvinceMap = invertProvinceMap(provinceMap);
	vector<EU3Country> oldCountries = sourceWorld.getCountries();
	for (unsigned int i = 0; i < countries.size(); i++)
	{
		int oldCapital = oldCountries[countries[i].getSourceCountryIndex()].getCapital();
		log("\n	EU3tag: %s	old capital: %4d", oldCountries[i].getTag().c_str(), oldCapital);
		inverseProvinceMapping::iterator itr = inverseProvinceMap.find(oldCapital);
		if (itr != inverseProvinceMap.end())
		{
			int newCapital = itr->second[0];
			countries[i].setCapital(newCapital);
			log("	new capital: %d", newCapital);
		}
	}
	log("\n");
}



static int stateId = 0;
void V2World::setupStates(stateMapping stateMap)
{
	list<V2Province> unassignedProvs;
	unassignedProvs.insert(unassignedProvs.begin(), provinces.begin(), provinces.end());

	list<V2Province>::iterator iter;
	while(unassignedProvs.size() > 0)
	{
		iter = unassignedProvs.begin();
		int		provId	= iter->getNum();
		string	owner		= iter->getOwner();

		if (owner == "")
		{
			unassignedProvs.erase(iter);
			continue;
		}

		V2State newState(stateId);
		stateId++;
		newState.addProvince(*iter);
		vector<int> neighbors	= stateMap[provId];
		bool colonial				= iter->isColonial();
		newState.setColonial(colonial);
		iter = unassignedProvs.erase(iter);

		for (unsigned int i = 0; i < neighbors.size(); i++)
		{
			for(iter = unassignedProvs.begin(); iter != unassignedProvs.end(); iter++)
			{
				if (iter->getNum() == neighbors[i])
				{
					if(iter->getOwner() == owner)
					{
						if ( (iter->isColonial()) == colonial)
						{
							newState.addProvince(*iter);
							iter = unassignedProvs.erase(iter);
						}
					}
				}
			}
		}

		for (vector<V2Country>::iterator iter2 = countries.begin(); iter2 != countries.end(); iter2++)
		{
			if (iter2->getTag() == owner)
			{
				iter2->addState(newState);
			}
		}
	}
}

void V2World::convertDiplomacy(EU3World sourceWorld, countryMapping countryMap)
{
	vector<EU3Agreement> agreements = sourceWorld.getDiplomacy().getAgreements();
	for (vector<EU3Agreement>::iterator itr = agreements.begin(); itr != agreements.end(); ++itr)
	{
		countryMapping::iterator newCountry1 = countryMap.find(itr->country1);
		if (newCountry1 == countryMap.end())
		{
			log("Error: EU3 Country %s used in diplomatic agreement doesn't exist\n", itr->country1.c_str());
			continue;
		}
		countryMapping::iterator newCountry2 = countryMap.find(itr->country2);
		if (newCountry2 == countryMap.end())
		{
			log("Error: EU3 Country %s used in diplomatic agreement doesn't exist\n", itr->country2.c_str());
			continue;
		}

		V2Country* v2Country1 = NULL;
		V2Country* v2Country2 = NULL;
		for (vector<V2Country>::iterator citr = countries.begin(); citr != countries.end(); ++citr)
		{
			if (citr->getTag() == newCountry1->second)
			{
				v2Country1 = &(*citr);
			}
			else if (citr->getTag() == newCountry2->second)
			{
				v2Country2 = &(*citr);
			}
			if (v2Country1 && v2Country2)
				break;
		}
		if (!v2Country1)
		{
			log("Error: Vic2 country %s used in diplomatic agreement doesn't exist\n", newCountry1->second.c_str());
			continue;
		}
		if (!v2Country2)
		{
			log("Error: Vic2 country %s used in diplomatic agreement doesn't exist\n", newCountry2->second.c_str());
			continue;
		}
		V2Relations* r1 = v2Country1->getRelations(newCountry2->second);
		if (!r1)
		{
			log("Error: Vic2 country %s has no relations with %s\n", newCountry1->second.c_str(), newCountry2->second.c_str());
			continue;
		}
		V2Relations* r2 = v2Country2->getRelations(newCountry1->second);
		if (!r2)
		{
			log("Error: Vic2 country %s has no relations with %s\n", newCountry2->second.c_str(), newCountry1->second.c_str());
			continue;
		}

		if ((itr->type == "royal_marriage" || itr->type == "guarantee"))
		{
			// influence level +1, but never exceed 4
			if (r1->getLevel() < 4)
				r1->setLevel(r1->getLevel() + 1);
		}
		if (itr->type == "royal_marriage")
		{
			// royal marriage is bidirectional; influence level +1, but never exceed 4
			if (r2->getLevel() < 4)
				r2->setLevel(r2->getLevel() + 1);
		}
		if ((itr->type == "sphere") || (itr->type == "vassal") || (itr->type == "union"))
		{
			// influence level = 5
			r1->setLevel(5);
			/* FIXME: is this desirable?
			// if relations are too poor, country2 will immediately eject country1's ambassadors at the start of the game
			// so, for stability's sake, give their relations a boost
			if (r1->getRelations() < 1)
				r1->setRelations(1);
			if (r2->getRelations() < 1)
				r2->setRelations(1);
			*/
		}
		if ((itr->type == "alliance") || (itr->type == "vassal"))
		{
			// copy agreement
			V2Agreement v2a;
			v2a.country1 = newCountry1->second;
			v2a.country2 = newCountry2->second;
			v2a.start_date = itr->start_date;
			v2a.type = itr->type;
			diplomacy.addAgreement(v2a);
		}
	}
}


void V2World::convertLeaders(EU3World sourceWorld, map<int,int>& leaderMap)
{
	LeaderTraits lt;
	lt.init();

	vector<EU3Country> oldCountries = sourceWorld.getCountries();
	for (unsigned int i = 0; i < countries.size(); ++i)
	{
		vector<EU3Leader> oldLeaders = oldCountries[countries[i].getSourceCountryIndex()].getLeaders();
		for (vector<EU3Leader>::iterator itr = oldLeaders.begin(); itr != oldLeaders.end(); ++itr)
		{
			V2Leader leader;
			leader.init(&(countries[i]));
			leader.setName(itr->getName());
			leader.setActivationDate(itr->getActivationDate());
			leader.setLand(itr->isLand());
			string personality = lt.getPersonality(itr->getFire(), itr->getShock(), itr->getManuever(), itr->getSiege());
			string background = lt.getBackground(itr->getFire(), itr->getShock(), itr->getManuever(), itr->getSiege());
			leader.setTraits(personality, background);
			countries[i].addLeader(leader);
			leaderMap[itr->getID()] = leader.getID();
		}
	}
}


static bool ProvinceRegimentCapacityPredicate(V2Province* prov1, V2Province* prov2)
{
	return (prov1->getAvailableSoldierCapacity() > prov2->getAvailableSoldierCapacity());
}

V2Province* V2World::getProvinceForExpeditionaryArmy(const V2Country& country)
{
	vector<V2Province*> candidates;
	for (vector<V2Province>::iterator pitr = provinces.begin(); pitr != provinces.end(); ++pitr)
	{
		if ((pitr->getOwner() == country.getTag()) && !pitr->wasColonised() 
			&& (pitr->getCulture() == country.getPrimaryCulture()) && (pitr->getPops("soldiers").size() > 0))
		{
			candidates.push_back(&(*pitr));
		}
	}
	if (candidates.size() > 0)
	{
		sort(candidates.begin(), candidates.end(), ProvinceRegimentCapacityPredicate);
		return candidates[0];
	}
	return NULL;
}


vector<int> V2World::getPortProvinces(vector<int> locationCandidates)
{
	// hack for naval bases.  not ALL naval bases are in port provinces, and if you spawn a navy at a naval base in
	// a non-port province, Vicky crashes....
	static vector<int> port_blacklist;
	if (port_blacklist.size() == 0)
	{
		int temp = 0;
		ifstream s("port_blacklist.txt");
		while (s.good() && !s.eof())
		{
			s >> temp;
			port_blacklist.push_back(temp);
		}
		s.close();
	}

	for (vector<int>::iterator litr = locationCandidates.begin(); litr != locationCandidates.end(); ++litr)
	{
		vector<int>::iterator black = std::find(port_blacklist.begin(), port_blacklist.end(), *litr);
		if (black != port_blacklist.end())
		{
			locationCandidates.erase(litr);
			break;
		}
	}
	for (vector<V2Province>::iterator pitr = provinces.begin(); pitr != provinces.end(); ++pitr)
	{
		for (vector<int>::iterator litr = locationCandidates.begin(); litr != locationCandidates.end(); ++litr)
		{
			if (pitr->getNum() == (*litr))
			{
				if (!pitr->isCoastal())
				{
					locationCandidates.erase(litr);
					--pitr;
					break;
				}
			}
		}
	}
	return locationCandidates;
}

// return values: 0 = success, -1 = retry from pool, -2 = do not retry
int V2World::addRegimentToArmy(V2Army* army, RegimentCategory rc, const inverseProvinceMapping& inverseProvinceMap, V2Country& country)
{
	V2Regiment reg((RegimentCategory)rc);
	int eu3Home = army->getSourceArmy()->getProbabilisticHomeProvince(rc);
	if (eu3Home == -1)
	{
		log("Error: army/navy %s has no valid home provinces for %s due to previous errors; dissolving to pool.\n", army->getName().c_str(), RegimentCategoryNames[rc]);
		return -2;
	}
	vector<int> homeCandidates = getV2ProvinceNums(inverseProvinceMap, eu3Home);
	if (homeCandidates.size() == 0)
	{
		log("Error: %s unit in army/navy %s has unmapped home province %d; dissolving to pool.\n", RegimentCategoryNames[rc], army->getName().c_str(), eu3Home);
		army->getSourceArmy()->blockHomeProvince(eu3Home);
		return -1;
	}
	V2Province* homeProvince;
	if (army->getNavy())
	{
		// Navies should only get homes in port provinces
		homeCandidates = getPortProvinces(homeCandidates);
		if (homeCandidates.size() == 0)
		{
			log("Error: %s in navy %s has EU3 home province %d which has no corresponding V2 port provinces; dissolving to pool.\n", RegimentCategoryNames[rc], army->getName().c_str(), eu3Home);
			army->getSourceArmy()->blockHomeProvince(eu3Home);
			return -1;
		}
		int homeProvinceID = homeCandidates[int(homeCandidates.size() * ((double)rand() / RAND_MAX))];
		for (vector<V2Province>::iterator pitr = provinces.begin(); pitr != provinces.end(); ++pitr)
		{
			if (pitr->getNum() == homeProvinceID)
			{
				homeProvince = &(*pitr);
				break;
			}
		}
	}
	else
	{
		// Armies should get a home in the candidate most capable of supporting them
		vector<V2Province*> sortedHomeCandidates;
		for (vector<V2Province>::iterator pitr = provinces.begin(); pitr != provinces.end(); ++pitr)
		{
			for (vector<int>::iterator nitr = homeCandidates.begin(); nitr != homeCandidates.end(); ++nitr)
			{
				if (pitr->getNum() == *nitr)
				{
					sortedHomeCandidates.push_back(&(*pitr));
				}
			}
			if (sortedHomeCandidates.size() == homeCandidates.size())
				break;
		}
		sort(sortedHomeCandidates.begin(), sortedHomeCandidates.end(), ProvinceRegimentCapacityPredicate);
		homeProvince = sortedHomeCandidates[0];
		// Armies need to be associated with pops
		if (homeProvince->getOwner() != country.getTag())
		{
			log("Error: V2 province %d is home for a %s %s regiment, but belongs to %s! Dissolving regiment to pool.\n", homeProvince->getNum(), country.getTag().c_str(), RegimentCategoryNames[rc], homeProvince->getOwner().c_str());
			// all provinces in a given province map have the same owner, so the source home was bad
			army->getSourceArmy()->blockHomeProvince(eu3Home);
			return -1;
		}
		int soldierPop = homeProvince->getSoldierPopForArmy();
		if (-1 == soldierPop)
		{
			// if the old home province was colonized and can't support the unit, try turning it into an "expeditionary" army
			if (homeProvince->wasColonised())
			{
				V2Province* expSender = getProvinceForExpeditionaryArmy(country);
				if (expSender)
				{
					int expSoldierPop = expSender->getSoldierPopForArmy();
					if (-1 != expSoldierPop)
					{
						homeProvince = expSender;
						soldierPop = expSoldierPop;
					}
				}
			}
		}
		if (-1 == soldierPop)
		{
			soldierPop = homeProvince->getSoldierPopForArmy(true);
			log("Error: Could not grow province %d soldier pops to support %s regiment in army %s. Regiment will be undersupported.\n", homeProvince->getNum(), RegimentCategoryNames[rc], army->getName().c_str());
		}
		reg.setPopID(soldierPop);
	}
	reg.setName(homeProvince->getRegimentName(rc));
	reg.setStrength(army->getSourceArmy()->getAverageStrength(rc) * (army->getNavy() ? 100.0 : 3.0));
	army->addRegiment(reg);
	return 0;
}

//#define TEST_V2_PROVINCES
void V2World::convertArmies(EU3World sourceWorld, provinceMapping provinceMap, const map<int,int>& leaderIDMap)
{
	// hack for naval bases.  not ALL naval bases are in port provinces, and if you spawn a navy at a naval base in
	// a non-port province, Vicky crashes....
	vector<int> port_whitelist;
	{
		int temp = 0;
		ifstream s("port_whitelist.txt");
		while (s.good() && !s.eof())
		{
			s >> temp;
			port_whitelist.push_back(temp);
		}
		s.close();
	}

	// get cost per regiment values
	double cost_per_regiment[num_reg_categories] = { 0.0 };
	Object*	obj2 = doParseFile("regiment_costs.txt");
	vector<Object*> objTop = obj2->getLeaves();
	if (objTop.size() == 0 || objTop[0]->getLeaves().size() == 0)
	{
		log("Error: regiment_costs.txt failed to parse.");
		printf("Error: regiment_costs.txt failed to parse.");
		exit(1);
	}
	for (int i = 0; i < num_reg_categories; ++i)
	{
		string regiment_cost = objTop[0]->getLeaf(RegimentCategoryNames[i]);
		cost_per_regiment[i] = atoi(regiment_cost.c_str());
	}

	inverseProvinceMapping inverseProvinceMap = invertProvinceMap(provinceMap);
	vector<EU3Country> sourceCountries = sourceWorld.getCountries();
	for (vector<V2Country>::iterator itr = countries.begin(); itr != countries.end(); ++itr)
	{
#ifndef TEST_V2_PROVINCES
		EU3Country* oldCountry = &sourceCountries[itr->getSourceCountryIndex()];

		// set up armies with whatever regiments they deserve, rounded down
		// and keep track of the remainders for later
		double countryRemainder[num_reg_categories] = { 0.0 };
		vector<EU3Army> sourceArmies = oldCountry->getArmies();
		for (vector<EU3Army>::iterator aitr = sourceArmies.begin(); aitr != sourceArmies.end(); ++aitr)
		{
			V2Army army;
			army.setSourceArmy(&(*aitr));
			army.setName(aitr->getName());
			army.setAtSea(aitr->getAtSea());

			map<int, int>::const_iterator lmapitr = leaderIDMap.find(aitr->getLeaderID());
			if (lmapitr != leaderIDMap.end())
			{
				army.setLeaderID(lmapitr->second);
			}

			for (int rc = infantry; rc < num_reg_categories; ++rc)
			{
				int typeStrength = aitr->getTotalTypeStrength((RegimentCategory)rc);
				if (typeStrength == 0) // no regiments of this type
					continue;

				// if we have ships, we must be a navy
				bool isNavy = (rc >= big_ship); 
				army.setNavy(isNavy);

				double regimentCount = typeStrength / cost_per_regiment[rc];
				int regimentsToCreate = (int)floor(regimentCount);
				double regimentRemainder = regimentCount - regimentsToCreate;
				countryRemainder[rc] += regimentRemainder;
				army.setArmyRemainders((RegimentCategory)rc, army.getArmyRemainder((RegimentCategory)rc) + regimentRemainder);

				double avg_strength = aitr->getAverageStrength((RegimentCategory)rc);
				for (int i = 0; i < regimentsToCreate; ++i)
				{
					if (addRegimentToArmy(&army, (RegimentCategory)rc, inverseProvinceMap, (*itr)) != 0)
					{
						// couldn't add, dissolve into pool
						countryRemainder[rc] += 1.0;
						army.setArmyRemainders((RegimentCategory)rc, army.getArmyRemainder((RegimentCategory)rc) + 1.0);
					}
				}
			}

			vector<int> locationCandidates = getV2ProvinceNums(inverseProvinceMap, aitr->getLocation());
			if (locationCandidates.size() == 0)
			{
				log("Error: Army or Navy %s assigned to unmapped province %d; dissolving to pool.\n", aitr->getName().c_str(), aitr->getLocation());
				int regimentCounts[num_reg_categories] = { 0 };
				army.getRegimentCounts(regimentCounts);
				for (int rc = infantry; rc < num_reg_categories; ++rc)
				{
					countryRemainder[rc] += regimentCounts[rc];
				}
				continue;
			}
			// guarantee that navies are assigned to provinces with naval bases
			if (army.getNavy())
			{
				locationCandidates = getPortProvinces(locationCandidates);
				if (locationCandidates.size() == 0)
				{
					log("Error: Navy %s assigned to EU3 province %d which has no corresponding V2 port provinces; dissolving to pool.\n", aitr->getName().c_str(), aitr->getLocation());
					int regimentCounts[num_reg_categories] = { 0 };
					army.getRegimentCounts(regimentCounts);
					for (int rc = infantry; rc < num_reg_categories; ++rc)
					{
						countryRemainder[rc] += regimentCounts[rc];
					}
					continue;
				}
			}
			int selectedLocation = locationCandidates[int(locationCandidates.size() * ((double)rand() / RAND_MAX))];
			if (army.getNavy())
			{
				vector<int>::iterator white = std::find(port_whitelist.begin(), port_whitelist.end(), selectedLocation);
				if (white == port_whitelist.end())
				{
					log("Warning: assigning navy to non-whitelisted port province %d.  If the save crashes, try blacklisting this province.\n", selectedLocation);
				}
			}
			army.setLocation(selectedLocation);
			itr->addArmy(army);
		}

		// allocate the remainders from the whole country to the armies according to their need, rounding up
		for (int rc = infantry; rc < num_reg_categories; ++rc)
		{
			if (countryRemainder[rc] > 0.0)
			{
				log("Allocating regiments of %s from the remainder pool for %s (total: %4lf)\n", RegimentCategoryNames[rc], itr->getTag().c_str(), countryRemainder[rc]);
			}
			while (countryRemainder[rc] > 0.0)
			{
				V2Army* army = itr->getArmyForRemainder((RegimentCategory)rc);
				if (!army)
				{
					log("Error: no suitable army or navy found for %s's pooled regiments of %s!\n", itr->getTag().c_str(), RegimentCategoryNames[rc]);
					break;
				}
				switch (addRegimentToArmy(army, (RegimentCategory)rc, inverseProvinceMap, (*itr)))
				{
				case 0: // success
					countryRemainder[rc] -= 1.0;
					army->setArmyRemainders((RegimentCategory)rc, army->getArmyRemainder((RegimentCategory)rc) - 1.0);
					break;
				case -1: // retry
					break;
				case -2: // do not retry
					log("Disqualifying army/navy %s from receiving more %s from the pool.\n", army->getName().c_str(), RegimentCategoryNames[rc]);
					army->setArmyRemainders((RegimentCategory)rc, -2000.0);
					break;
				}
			}
		}

#else // ifdef TEST_V2_PROVINCES
		// output one big ship to each V2 province that's neither whitelisted nor blacklisted, but only 10 at a time per nation
		// output from this mode is used to build whitelist and blacklist files
		int n_tests = 0;
		for (vector<V2Province>::iterator pitr = provinces.begin(); (pitr != provinces.end()) && (n_tests < 50); ++pitr)
		{
			if ((pitr->getOwner() == itr->getTag()) && pitr->isCoastal())
			{
				vector<int>::iterator black = std::find(port_blacklist.begin(), port_blacklist.end(), pitr->getNum());
				if (black != port_blacklist.end())
					continue;

				V2Army army;
				army.setName("V2 Test Navy");
				army.setAtSea(0);
				army.setNavy(true);
				army.setLocation(pitr->getNum());
				V2Regiment reg(big_ship);
				reg.setStrength(100);
				army.addRegiment(reg);
				itr->addArmy(army);

				vector<int>::iterator white = std::find(port_whitelist.begin(), port_whitelist.end(), pitr->getNum());
				if (white == port_whitelist.end())
				{
					++n_tests;
					ofstream s("port_greylist.txt", ios_base::app);
					s << pitr->getNum() << "\n";
					s.close();
				}
			}
		}
		log("Output %d test ships.\n", n_tests);
#endif
	}
}


void V2World::convertTechs(EU3World sourceWorld)
{
	vector<EU3Country> sourceCountries = sourceWorld.getCountries();
	
	double oldLandMean;
	double landMean;
	double oldLandS = 0.0;
	double newLandS;
	double highestLand;

	double oldNavalMean;
	double navalMean;
	double oldNavalS = 0.0;
	double newNavalS;
	double highestNaval;

	double oldTradeMean;
	double tradeMean;
	double oldTradeS = 0.0;
	double newTradeS;
	double highestTrade;

	double oldProductionMean;
	double productionMean;
	double oldProductionS = 0.0;
	double newProductionS;
	double highestProduction;

	double oldGovernmentMean;
	double governmentMean;
	double oldGovernmentS = 0.0;
	double newGovernmentS;
	double highestGovernment;

	int num = 2;
	unsigned int i = 0;
	while (sourceCountries[i].getProvinces().size() == 0)
	{
		i++;
	}
	highestLand			= oldLandMean			= landMean			= sourceCountries[i].getLandTech();
	highestNaval		= oldNavalMean			= navalMean			= sourceCountries[i].getNavalTech();
	highestTrade		= oldTradeMean			= tradeMean			= sourceCountries[i].getTradeTech();
	highestProduction	= oldProductionMean	= productionMean	= sourceCountries[i].getProductionTech();
	highestGovernment	= oldGovernmentMean	= governmentMean	= sourceCountries[i].getGovernmentTech();

	for (i++; i < sourceCountries.size(); i++)
	{
		if (sourceCountries[i].getProvinces().size() == 0)
		{
			continue;
		}
		double newTech	= sourceCountries[i].getLandTech();
		landMean			= oldLandMean + ((newTech - oldLandMean) / num);
		newLandS			= oldLandS + ((newTech - oldLandMean) * (newTech - landMean));
		oldLandMean		= landMean; 
		oldLandS			= newLandS;
		if (newTech > highestLand)
		{
			highestLand = newTech;
		}

		newTech			= sourceCountries[i].getNavalTech();
		navalMean		= oldNavalMean + ((newTech - oldNavalMean) / num);
		newNavalS		= oldNavalS + ((newTech - oldNavalMean) * (newTech - navalMean));
		oldNavalMean	= navalMean; 
		oldNavalS		= newNavalS;
		if (newTech > highestNaval)
		{
			highestNaval = newTech;
		}

		newTech			= sourceCountries[i].getTradeTech();
		tradeMean		= oldTradeMean + ((newTech - oldTradeMean) / num);
		newTradeS		= oldTradeS + ((newTech - oldTradeMean) * (newTech - tradeMean));
		oldTradeMean	= tradeMean; 
		oldTradeS		= newTradeS;
		if (newTech > highestTrade)
		{
			highestTrade = newTech;
		}

		newTech				= sourceCountries[i].getProductionTech();
		productionMean		= oldProductionMean + ((newTech - oldProductionMean) / num);
		newProductionS		= oldProductionS + ((newTech - oldProductionMean) * (newTech - productionMean));
		oldProductionMean	= productionMean; 
		oldProductionS		= newProductionS;
		if (newTech > highestProduction)
		{
			highestProduction = newTech;
		}

		newTech				= sourceCountries[i].getGovernmentTech();
		governmentMean		= oldGovernmentMean + ((newTech - oldGovernmentMean) / num);
		newGovernmentS		= oldGovernmentS + ((newTech - oldGovernmentMean) * (newTech - governmentMean));
		oldGovernmentMean	= governmentMean; 
		oldGovernmentS		= newGovernmentS;
		if (newTech > highestGovernment)
		{
			highestGovernment = newTech;
		}

		num++;
	}

	double landStdDev			= sqrt( (num > 1) ? (newLandS/(num - 1)) : 0.0 );
	double navalStdDev		= sqrt( (num > 1) ? (newNavalS/(num - 1)) : 0.0 );
	double tradeStdDev		= sqrt( (num > 1) ? (newTradeS/(num - 1)) : 0.0 );
	double productionStdDev	= sqrt( (num > 1) ? (newProductionS/(num - 1)) : 0.0 );
	double governmentStdDev	= sqrt( (num > 1) ? (newGovernmentS/(num - 1)) : 0.0 );

	double landScale			= (5		* landStdDev)			/ (highestLand			- landMean);
	double navalScale			= (5		* navalStdDev)			/ (highestNaval		- navalMean);
	double tradeScale			= (5		* tradeStdDev)			/ (highestTrade		- tradeMean);
	double productionScale	= (4.5	* productionStdDev)	/ (highestProduction	- productionMean);
	double governmentScale	= (4.5	* governmentStdDev)	/ (highestGovernment	- governmentMean);

	for (unsigned int i = 0; i < countries.size(); i++)
	{
		double armyTech = ((landScale * (sourceCountries[countries[i].getSourceCountryIndex()].getLandTech() - landMean) / landStdDev) + 4.5);
		countries[i].setArmyTech(armyTech);
		log("%s has army tech of %f\n", countries[i].getTag().c_str(), armyTech);

		double navyTech = (navalScale * (sourceCountries[countries[i].getSourceCountryIndex()].getNavalTech() - navalMean) / navalStdDev) + 5;
		countries[i].setNavyTech(navyTech);
		log("%s has navy tech of %f\n", countries[i].getTag().c_str(), navyTech);

		double commerceTech = (tradeScale * (sourceCountries[countries[i].getSourceCountryIndex()].getTradeTech() - tradeMean) / tradeStdDev) + 5;
		countries[i].setCommerceTech(commerceTech);
		log("%s has commerce tech of %f\n", countries[i].getTag().c_str(), commerceTech);

		double industryTech = (productionScale * (sourceCountries[countries[i].getSourceCountryIndex()].getProductionTech() - productionMean) / productionStdDev) + 5;
		countries[i].setIndustryTech(industryTech);
		log("%s has industry tech of %f\n", countries[i].getTag().c_str(), industryTech);

		double cultureTech = ((governmentScale * (sourceCountries[countries[i].getSourceCountryIndex()].getGovernmentTech() - governmentMean) / governmentStdDev) + 4.5);
		countries[i].setCultureTech(cultureTech);
		log("%s has culture tech of %f\n", countries[i].getTag().c_str(), cultureTech);
	}

	int numRomanticLit = 0;
	int numRomanticArt = 0;
	int numRomanticMusic = 0;
	int numPressureChambers = 0;
	for (unsigned int i = 0; i < countries.size(); i++)
	{
		if (countries[i].getInventionState(romanticist_literature) == active)
		{
			numRomanticLit++;
		}
		if (countries[i].getInventionState(romanticist_art) == active)
		{
			numRomanticArt++;
		}
		if (countries[i].getInventionState(romanticist_music) == active)
		{
			numRomanticMusic++;
		}
		if (countries[i].getInventionState(pressure_chambers_for_thorax_surgery) == active)
		{
			numPressureChambers++;
		}
	}

	double romanticLitPrestige = 0;
	for (int i = 1; i <= numRomanticLit; i++)
	{
		romanticLitPrestige += 1.0/i;
	}
	romanticLitPrestige *= 20;
	romanticLitPrestige /= numRomanticLit;

	double romanticArtPrestige = 0;
	for (int i = 1; i <= numRomanticArt; i++)
	{
		romanticArtPrestige += 1.0/i;
	}
	romanticArtPrestige *= 20;
	romanticArtPrestige /= numRomanticArt;

	double romanticMusicPrestige = 0;
	for (int i = 1; i <= numRomanticMusic; i++)
	{
		romanticMusicPrestige += 1.0/i;
	}
	romanticMusicPrestige *= 20;
	romanticMusicPrestige /= numRomanticMusic;

	double pressureChambersPrestige = 0;
	for (int i = 1; i <= numPressureChambers; i++)
	{
		pressureChambersPrestige += 1.0/i;
	}
	pressureChambersPrestige *= 50;
	pressureChambersPrestige /= numPressureChambers;

	for (unsigned int i = 0; i < countries.size(); i++)
	{
		if (countries[i].getInventionState(romanticist_literature) == active)
		{
			countries[i].addPrestige(romanticLitPrestige);
		}
		if (countries[i].getInventionState(romanticist_art) == active)
		{
			countries[i].addPrestige(romanticArtPrestige);
		}
		if (countries[i].getInventionState(romanticist_music) == active)
		{
			countries[i].addPrestige(romanticMusicPrestige);
		}
		if (countries[i].getInventionState(pressure_chambers_for_thorax_surgery) == active)
		{
			countries[i].addPrestige(pressureChambersPrestige);
		}
	}
}


void V2World::allocateFactories(EU3World sourceWorld, V2FactoryFactory& factoryBuilder)
{
	vector<EU3Country> sourceCountries = sourceWorld.getCountries();
	double productionMean = 0.0f;
	int num = 1;
	for (vector<EU3Country>::iterator itr = sourceCountries.begin(); itr != sourceCountries.end(); ++itr)
	{
		if (itr->getProvinces().size() == 0)
			continue;
		if (itr->getTechGroup() != "western")
			continue;

		double prodTech = itr->getProductionTech();
		productionMean += ((prodTech - productionMean) / num);
		++num;
	}

	vector<pair<double, V2Country*>> weightedCountries;
	double totalIndWeight = 0.0;
	for (vector<V2Country>::iterator itr = countries.begin(); itr != countries.end(); ++itr)
	{
		if (!itr->isCivilized())
			continue;

		EU3Country* source = &sourceCountries[itr->getSourceCountryIndex()];
		if (source->getProvinces().size() == 0)
			continue;

		// modified manufactory weight follows diminishing returns curve y = x^(3/4)+log((x^2)/5+1)
		int manuCount = source->getManufactoryCount();
		double manuWeight = pow(manuCount, 0.75) + log((manuCount * manuCount) / 5.0 + 1.0);
		double industryWeight = (source->getProductionTech() - productionMean) + manuWeight;
		// having one manufactory and average tech is not enough; you must have more than one, or above-average tech
		if (industryWeight > 1.0)
		{
			weightedCountries.push_back(pair<double, V2Country*>(industryWeight, &(*itr)));
			totalIndWeight += industryWeight;
		}
	}
	if (weightedCountries.size() < 1)
	{
		log("Error: no countries are able to accept factories!\n");
		return;
	}
	sort(weightedCountries.begin(), weightedCountries.end());

	deque<V2Factory> factoryList = factoryBuilder.buildFactories();
	vector<pair<int, V2Country*>> factoryCounts;
	for (vector<pair<double, V2Country*>>::iterator itr = weightedCountries.begin(); itr != weightedCountries.end(); ++itr)
	{
		int factories = int(ceil((itr->first / totalIndWeight) * factoryList.size()));
		log("%s has industrial weight %2.2lf granting %d factories\n", itr->second->getTag().c_str(), itr->first, factories);
		factoryCounts.push_back(pair<int, V2Country*>(factories, itr->second));
	}

	vector<pair<int, V2Country*>>::iterator lastReceptiveCountry = factoryCounts.begin();
	vector<pair<int, V2Country*>>::iterator citr = factoryCounts.begin();
	while (factoryList.size() > 0)
	{
		bool accepted = false;
		if (citr->first > 0) // can take more factories
		{
			for (deque<V2Factory>::iterator qitr = factoryList.begin(); qitr != factoryList.end(); ++qitr)
			{
				if (citr->second->addFactory(*qitr))
				{
					--(citr->first);
					lastReceptiveCountry = citr;
					accepted = true;
					factoryList.erase(qitr);
					break;
				}
			}
		}
		if (!accepted && citr == lastReceptiveCountry)
		{
			log("No countries will accept any of the remaining factories!");
			for (deque<V2Factory>::iterator qitr = factoryList.begin(); qitr != factoryList.end(); ++qitr)
			{
				log("\n\t%s", qitr->getTypeName().c_str());
			}
			log("\n");
			break;
		}
		if (++citr == factoryCounts.end())
			citr = factoryCounts.begin(); // loop around to beginning
	}
}


void V2World::output(FILE* output)
{
	outputHeader(output);
	outputTempHeader(output);
	for (unsigned int i = 0; i < provinces.size(); i++)
	{
		provinces[i].output(output);
	}
	for (unsigned int i = 0; i < countries.size(); i++)
	{
		countries[i].output(output);
	}
	diplomacy.output(output);
}


void V2World::outputHeader(FILE* output)
{
	fprintf(output, "date=\"1835.12.1\"\n");
	fprintf(output, "automate_trade=no\n");
	fprintf(output, "automate_sliders=0\n");
	fprintf(output, "unit=%d\n", V2ArmyID().id);
	fprintf(output, "state=%d\n", stateId);
	fprintf(output, "start_date=\"1835.12.31\"\n");
	fprintf(output, "start_pop_index=%d\n", getNextPopId());
	fprintf(output, "worldmarket=\n");
	fprintf(output, "{\n");
	fprintf(output, "	worldmarket_pool=\n");
	fprintf(output, "	{\n");
	fprintf(output, "		ammunition=0.46634\n");
	fprintf(output, "		small_arms=1.01407\n");
	fprintf(output, "		artillery=1.43967\n");
	fprintf(output, "		canned_food=4.16602\n");
	fprintf(output, "		cotton=966.92960\n");
	fprintf(output, "		dye=785.78574\n");
	fprintf(output, "		wool=1794.33109\n");
	fprintf(output, "		silk=192.24536\n");
	fprintf(output, "		coal=1698.31195\n");
	fprintf(output, "		sulphur=201.85480\n");
	fprintf(output, "		iron=498.86673\n");
	fprintf(output, "		timber=1038.01337\n");
	fprintf(output, "		tropical_wood=258.42496\n");
	fprintf(output, "		precious_metal=17.16943\n");
	fprintf(output, "		steel=395.23450\n");
	fprintf(output, "		cement=3.23746\n");
	fprintf(output, "		machine_parts=3.84375\n");
	fprintf(output, "		glass=52.54648\n");
	fprintf(output, "		fertilizer=66.62204\n");
	fprintf(output, "		explosives=18.46304\n");
	fprintf(output, "		clipper_convoy=0.05063\n");
	fprintf(output, "		steamer_convoy=2.83932\n");
	fprintf(output, "		fabric=139.08908\n");
	fprintf(output, "		lumber=101.34860\n");
	fprintf(output, "		paper=3.35300\n");
	fprintf(output, "		cattle=813.88202\n");
	fprintf(output, "		fish=1535.25745\n");
	fprintf(output, "		fruit=2686.31516\n");
	fprintf(output, "		grain=4360.54388\n");
	fprintf(output, "		tobacco=2200.08780\n");
	fprintf(output, "		tea=2673.66977\n");
	fprintf(output, "		coffee=1334.11459\n");
	fprintf(output, "		opium=1118.43161\n");
	fprintf(output, "		wine=7.32648\n");
	fprintf(output, "		liquor=0.69968\n");
	fprintf(output, "		regular_clothes=34.41812\n");
	fprintf(output, "		luxury_clothes=0.40475\n");
	fprintf(output, "		furniture=0.29919\n");
	fprintf(output, "		luxury_furniture=0.38611\n");
	fprintf(output, "	}\n");
	fprintf(output, "	price_pool=\n");
	fprintf(output, "	{\n");
	fprintf(output, "		ammunition=17.52002\n");
	fprintf(output, "		small_arms=37.00000\n");
	fprintf(output, "		artillery=60.00000\n");
	fprintf(output, "		canned_food=16.00000\n");
	fprintf(output, "		barrels=98.00000\n");
	fprintf(output, "		aeroplanes=110.00000\n");
	fprintf(output, "		cotton=1.97998\n");
	fprintf(output, "		dye=11.97998\n");
	fprintf(output, "		wool=0.67999\n");
	fprintf(output, "		silk=10.00000\n");
	fprintf(output, "		coal=2.28998\n");
	fprintf(output, "		sulphur=6.02002\n");
	fprintf(output, "		iron=3.47998\n");
	fprintf(output, "		timber=0.89999\n");
	fprintf(output, "		tropical_wood=5.37997\n");
	fprintf(output, "		rubber=7.00000\n");
	fprintf(output, "		oil=12.00000\n");
	fprintf(output, "		precious_metal=7.50000\n");
	fprintf(output, "		steel=4.67999\n");
	fprintf(output, "		cement=15.97998\n");
	fprintf(output, "		machine_parts=36.47998\n");
	fprintf(output, "		glass=2.92001\n");
	fprintf(output, "		fuel=25.00000\n");
	fprintf(output, "		fertilizer=10.00000\n");
	fprintf(output, "		explosives=20.02002\n");
	fprintf(output, "		clipper_convoy=42.00000\n");
	fprintf(output, "		steamer_convoy=64.97998\n");
	fprintf(output, "		electric_gear=16.00000\n");
	fprintf(output, "		fabric=1.82001\n");
	fprintf(output, "		lumber=1.02002\n");
	fprintf(output, "		paper=3.42001\n");
	fprintf(output, "		cattle=1.97998\n");
	fprintf(output, "		fish=1.47998\n");
	fprintf(output, "		fruit=1.77997\n");
	fprintf(output, "		grain=2.17999\n");
	fprintf(output, "		tobacco=1.10001\n");
	fprintf(output, "		tea=2.60001\n");
	fprintf(output, "		coffee=2.07999\n");
	fprintf(output, "		opium=3.20001\n");
	fprintf(output, "		automobiles=70.00000\n");
	fprintf(output, "		telephones=16.00000\n");
	fprintf(output, "		wine=9.72003\n");
	fprintf(output, "		liquor=6.42001\n");
	fprintf(output, "		regular_clothes=5.82001\n");
	fprintf(output, "		luxury_clothes=65.02002\n");
	fprintf(output, "		furniture=4.92001\n");
	fprintf(output, "		luxury_furniture=59.02002\n");
	fprintf(output, "		radio=16.00000\n");
	fprintf(output, "	}\n");
	fprintf(output, "	last_price_history=\n");
	fprintf(output, "	{\n");
	fprintf(output, "	}\n");
	fprintf(output, "	supply_pool=\n");
	fprintf(output, "	{\n");
	fprintf(output, "	}\n");
	fprintf(output, "	last_supply_pool=\n");
	fprintf(output, "	{\n");
	fprintf(output, "	}\n");
	fprintf(output, "	price_change=\n");
	fprintf(output, "	{\n");
	fprintf(output, "	}\n");
	fprintf(output, "	discovered_goods=\n");
	fprintf(output, "	{\n");
	fprintf(output, "		ammunition=1.00000\n");
	fprintf(output, "		small_arms=1.00000\n");
	fprintf(output, "		artillery=1.00000\n");
	fprintf(output, "		canned_food=1.00000\n");
	fprintf(output, "		cotton=1.00000\n");
	fprintf(output, "		dye=1.00000\n");
	fprintf(output, "		wool=1.00000\n");
	fprintf(output, "		silk=1.00000\n");
	fprintf(output, "		coal=1.00000\n");
	fprintf(output, "		sulphur=1.00000\n");
	fprintf(output, "		iron=1.00000\n");
	fprintf(output, "		timber=1.00000\n");
	fprintf(output, "		tropical_wood=1.00000\n");
	fprintf(output, "		rubber=1.00000\n");
	fprintf(output, "		oil=1.00000\n");
	fprintf(output, "		precious_metal=1.00000\n");
	fprintf(output, "		steel=1.00000\n");
	fprintf(output, "		cement=1.00000\n");
	fprintf(output, "		machine_parts=1.00000\n");
	fprintf(output, "		glass=1.00000\n");
	fprintf(output, "		fertilizer=1.00000\n");
	fprintf(output, "		explosives=1.00000\n");
	fprintf(output, "		clipper_convoy=1.00000\n");
	fprintf(output, "		steamer_convoy=1.00000\n");
	fprintf(output, "		fabric=1.00000\n");
	fprintf(output, "		lumber=1.00000\n");
	fprintf(output, "		paper=1.00000\n");
	fprintf(output, "		cattle=1.00000\n");
	fprintf(output, "		fish=1.00000\n");
	fprintf(output, "		fruit=1.00000\n");
	fprintf(output, "		grain=1.00000\n");
	fprintf(output, "		tobacco=1.00000\n");
	fprintf(output, "		tea=1.00000\n");
	fprintf(output, "		coffee=1.00000\n");
	fprintf(output, "		opium=1.00000\n");
	fprintf(output, "		wine=1.00000\n");
	fprintf(output, "		liquor=1.00000\n");
	fprintf(output, "		regular_clothes=1.00000\n");
	fprintf(output, "		luxury_clothes=1.00000\n");
	fprintf(output, "		furniture=1.00000\n");
	fprintf(output, "		luxury_furniture=1.00000\n");
	fprintf(output, "	}\n");
	fprintf(output, "	actual_sold=\n");
	fprintf(output, "	{\n");
	fprintf(output, "	}\n");
	fprintf(output, "	actual_sold_world=\n");
	fprintf(output, "	{\n");
	fprintf(output, "	}\n");
	fprintf(output, "	real_demand=\n");
	fprintf(output, "	{\n");
	fprintf(output, "	}\n");
	fprintf(output, "	demand=\n");
	fprintf(output, "	{\n");
	fprintf(output, "	}\n");
	fprintf(output, "	player_balance=\n");
	fprintf(output, "	{\n");
	fprintf(output, "	}\n");
	fprintf(output, "}\n");
}
