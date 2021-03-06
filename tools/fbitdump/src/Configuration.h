/**
 * \file Configuration.h
 * \author Petr Velan <petr.velan@cesnet.cz>
 * \brief Header of class for managing user input of fbitdump
 *
 * Copyright (C) 2011 CESNET, z.s.p.o.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of the Company nor the names of its contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * ALTERNATIVELY, provided that this notice is retained in full, this
 * product may be distributed under the terms of the GNU General Public
 * License (GPL) version 2 or later, in which case the provisions
 * of the GPL apply INSTEAD OF those given above.
 *
 * This software is provided ``as is, and any express or implied
 * warranties, including, but not limited to, the implied warranties of
 * merchantability and fitness for a particular purpose are disclaimed.
 * In no event shall the company or contributors be liable for any
 * direct, indirect, incidental, special, exemplary, or consequential
 * damages (including, but not limited to, procurement of substitute
 * goods or services; loss of use, data, or profits; or business
 * interruption) however caused and on any theory of liability, whether
 * in contract, strict liability, or tort (including negligence or
 * otherwise) arising in any way out of the use of this software, even
 * if advised of the possibility of such damage.
 *
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include "pugixml.hpp"
#include "typedefs.h"
#include "Column.h"
#include "Resolver.h"
#include <queue>
#include "plugins/plugin_header.h"
namespace fbitdump {

/** Acceptable command-line parameters */
#define OPTSTRING "hVlaA::r:f:n:c:D:Ns:qeIM:m::R:o:v:Z:t:i::d::C:Tp:SO"

#define CONFIG_XML "/usr/share/fbitdump/fbitdump.xml"

/**
 * \brief Struct with pointers to plugin functions
 */
struct pluginConf {
	pluginConf(): init(NULL), close(NULL), format(NULL), /* Structure initialization */
				  parse(NULL), info(NULL), handle(NULL) {};
	int (*init) (void);     		/**< Plugin initialization */
	void (*close) (void); 			/**< Plugin close */
	void (*format)(const union plugin_arg *, int, char*); 	/**< Format data */
	void (*parse)(char *, char*); 	/**< Parse input */
        char *(*info) (void);           /**< Plugin description */
	void *handle; 					/**< Plugin handle */
};

typedef std::map<std::string, pluginConf> pluginMap;

/**
 * \brief Class handling command line configuration
 *
 * This class parses and processes command line configuration
 * Based on the configuration it prepares Columns, list of fastbit parts to load, and other things
 * This class is often passed to constructors of others
 */
class Configuration
{
public:

	/**
	 * \brief Configuration constructor
	 */
	Configuration();

	/**
	 * \brief Parse user input and initialise tables and columns
	 *
	 * @param argc number of arguments
	 * @param argv array of arguments
	 * @return 0 on success, 1 when help was printed and program should stop
	 */
    int init(int argc, char *argv[]) throw (std::invalid_argument);

    /**
     * \brief Returns vector of table parts names
     *
     * @return Vector of strings with parts names
     */
    const stringVector getPartsNames() const;

    /**
     * \brief Returns filter string as passed by user
     *
     * @return Strings containing filter from user
     */
    std::string getFilter() const;

    /**
     * \brief Returns set of fastbit column names containing columns
     * to be used in select clause of aggregation query
     * These columns are used to aggregate by
     *
     * @return stringSet
     */
    const stringSet getAggregateColumns() const;

    /**
     * \brief Returns set of column names containing columns
     * to be used in select clause of aggregation query
     * These columns are used with summary function (sum, min, max)
     *
     * @return stringSet
     */
    const stringSet getSummaryColumns() const;

    /**
     * \brief Returns column to order by
     *
     * Column might be null when order by is not set by -m option
     *
     * @return Column to order by
     */
    const Column* getOrderByColumn() const;

    /**
     * \brief Returns true when option for printing plain numbers was passed
     *
     * @return True when option for printing only plain numbers was passed,
     * else otherwise
     */
    bool getPlainNumbers() const;

    /**
     * \brief Returns ceil limit of records to print
     * @return Maximum records number to print
     */
    size_t getMaxRecords() const;

    /**
     * \brief Returns true when record aggregation is required
     *
     * @return True when agregation is on, false otherwise
     */
    bool getAggregate() const;

    /**
     * \brief Returns true when quiet mode is requested
     * No header and bottom statistics should be printed
     *
     * @return True when quiet mode is requested
     */
    bool getQuiet() const;

    /**
     * \brief Returns vector of Columns
     * Columns are created based on format string and configuration XML
     *
     * @return Vector of columns
     */
    const columnVector& getColumns() const;

    /**
     * \brief This method returns true if user started application with -m option
     *
     * @return true if option "-m" was specified, false otherwise
     */
    bool getOptionm() const;

	/**
     * \brief Returns string with time window start
     *
     * @return string with time window start, empty if none used
     */
    const std::string getTimeWindowStart() const;

    /**
     * \brief Returns string with time window end
     *
     * @return string with time window end, empty if none used
     */
    const std::string getTimeWindowEnd() const;

    /**
     * \brief Returns resolver
     *
     * @return object which provides DNS resolving functionality
     */
    Resolver *getResolver() const;

    /**
     * \brief Get order direction for fastbit order by function
     *
     * @return string "ASC" or "DESC"
     */
    bool getOrderAsc() const;

    /**
     * \brief Return true if statistics were requested
     *
     * @return true if statistics were requested
     */
    bool getStatistics() const;

    /**
     * \brief Provides access to aggregation columns aliases
     *
     * @return aggregation columns aliases
     */
    const stringSet& getAggregateColumnsAliases() const;

    /**
     * \brief Returns XML configuration document
     *
     * @return XML configuration document
     */
    const pugi::xml_document& getXMLConfiguration() const;

    /**
     * \brief Returns columns that should be used as summary for statistics
     *
     * @return Columns that should be used as summary for statistics
     */
    columnVector getStatisticsColumns() const;

    /**
     * \brief Returns true when extended bootm stats were requested
     *
     * @return true when extended bootm stats were requested
     */
    bool getExtendedStats() const;

    /**
     * \brief Returns true when creation of indexes was requested
     *
     * @return true when creation of indexes was requested
     */
    bool getCreateIndexes() const;

    /**
     * \brief Returns true when deletion of indexes was requested
     *
     * @return true when deletion of indexes was requested
     */
    bool getDeleteIndexes() const;

    /**
     * \brief Returns set of index column names to work with
     *
     * @return set of index column names to work with
     */
    stringSet getColumnIndexes() const;

    /**
     * \brief Returns true when template information was requested
     *
     * @return true when template information was requested
     */
    bool getTemplateInfo() const;

    /**
     * \brief Class destructor
     */
    ~Configuration();
	
	/**
	 * \ brief For communication with expire daemon
	 */
	std::string pipe_name;
	
	static Configuration * instance;

    pluginMap plugins; /**< active plugins */

    /**
     * \brief Unload plugins
     */
    void unloadModules();
private:
    /**
     * \brief Load plugins for parsing input and formatting output
     */
	void loadModules();

    /**
     * \brief Prints help to standard output
     */
    void help() const;

    /**
     * \brief Returns path to configuration XML
     * @return Path to configuration XML
     */
    const char* getXmlConfPath() const;

    /**
     * \brief Read parts from specified table directories
     *
     * Writes parts to instance variable "parts"
     *
     * @param tables fastbit tables to be used
     */
    void searchForTableParts(stringVector &tables) throw (std::invalid_argument);

	/**
	 * \brief Parse output format string
	 * @param format output format string
	 * @param orderby alias of sorting column
	 */
	void parseFormat(std::string format, std::string &orderby);

    /**
     * \brief Process -M option from getopt()
     *
     * @param tables vector containing names of input directories
     * @param optarg optarg for -M option
     * @param optionr optarg for -r option
     *
     * Local variable "tables", specified in init() method, will contain input
     * directories specified by -M option.
     */
    void processMOption(stringVector &tables, const char *optarg, std::string &optionr);

    /**
     * \brief Process -R option from getopt()
     *
     * @param tables vector containing names of input directories
     * @param optarg optarg for -R option
     *
     * Local variable "tables", specified in init() method, will contain input
     * directories specified by -R option.
     */
    void processROption(stringVector &tables, const char *optarg);

    /**
     * \brief Process optional param of -m option
     *
     * Create column to order by
     *
     * @param order name of the column to order by
     */
    void processmOption(std::string &order);

    /**
     * \brief Parse argument for aggregation
     *
     * @param arg comma separated list of aliases
     */
    void parseAggregateArg(char *arg) throw (std::invalid_argument);

    /**
     * \brief Get index columns names from columns specified by aliases
     *
     * @param arg string with comma separated column aliases
     */
    void parseIndexColumns(char *arg);

    /**
     * \brief Check and load named output format from configuration file
     *
     * Uses and modifies this->format variable
     */
    void loadOutputFormat() throw (std::invalid_argument);

    /**
     * \brief Check that a directory exists and push it into the list if it does
     *
     * @param dir Directory to push
     * @param list Vector to push the directory into
     */
    void pushCheckDir(std::string &dir, std::vector<std::string> &list);

    /**
     * \brief Print available output formats
     */
    void printOutputFormats();
    
    /**
     * \brief Print informations about loaded plugins
     */
    void printModules();

    stringVector parts;                 /**< Fastbit parts paths to be used*/
    stringSet aggregateColumnsAliases;  /**< Aggregate columns aliases set */
	uint64_t maxRecords;                /**< Limit number of printed records */
	bool plainNumbers;                  /**< Don't convert protocol numbers to strings*/
	bool aggregate;                     /**< Are we in aggreagate mode? */
	bool quiet;                         /**< Don't print header and statistics */
	std::string filter;                 /**< User specified filter string */
	std::string format;                 /**< Output format*/
	columnVector columns;               /**< Vector of columns to print */
	std::string firstdir;               /**< First table (directory) user wants to work with */
	std::string lastdir;                /**< Last table (directory) user wants to work with */
	bool optm;                          /**< Indicates whether user specified "-m" option or not */
	Column *orderColumn;	 			/**< Column specified using -m value, default is %ts */
	std::string timeWindow;             /**< Time window */
	Resolver *resolver;                 /**< DNS resolver */
	bool statistics;					/**< Option to generate statistics was used */
	bool orderAsc;						/**< Order column increasingly, default is true */
	pugi::xml_document doc;				/**< XML configuration document */
	bool extendedStats;					/**< Print extended bottom stats */
	bool createIndexes;					/**< Create indexes specified by indexColumns */
	bool deleteIndexes;					/**< Delete indexes specified by indexColumns */
	stringSet indexColumns;				/**< Indexes specified by -i or -d option. Empty means all */
	std::string configFile;				/**< Configuration file path */
	bool templateInfo;					/**< Print information about used templates */
}; /* end of Configuration class */

} /* end of fbitdump namespace */

#endif /* CONFIGURATION_H_ */
