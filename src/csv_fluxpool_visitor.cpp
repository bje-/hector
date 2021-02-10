/* Hector -- A Simple Climate Model
   Copyright (C) 2014-2015  Battelle Memorial Institute

   Please see the accompanying file LICENSE.md for additional licensing
   information.
*/
/*
 *  csv_fluxpool_visitor.cpp
 *  hector
 *
 *  Created by Skylar Gering on 21 January 2021.
 *
 */

#include <fstream>
#include <boost/lexical_cast.hpp>
#include "core.hpp"
#include "simpleNbox.hpp"
#include "csv_fluxpool_visitor.hpp"
#include "unitval.hpp"
#include "fluxpool.hpp"
#include "h_util.hpp"

namespace Hector {

using namespace std;

//------------------------------------------------------------------------------
/*! \brief Constructor
 *  \param filename The file to write the csv output to.
 */
CSVFluxPoolVisitor::CSVFluxPoolVisitor( ostream& outputStream, const bool printHeader )
:csvFile( outputStream )
{
    if( printHeader ) {
        // Print table header
        csvFile << "year" << DELIMITER
                << "pool_name" << DELIMITER << "pool_value" << DELIMITER << "pool_units" << DELIMITER
                << "source_name" << DELIMITER << "source_fraction" <<endl;
    }
}

//------------------------------------------------------------------------------
/*! \brief Destructor
 */
CSVFluxPoolVisitor::~CSVFluxPoolVisitor() {
}

//------------------------------------------------------------------------------
// documentation is inherited
bool CSVFluxPoolVisitor::shouldVisit( const bool in_spinup, const double date ) {
    // visit all non-spinup model periods
    datestring = boost::lexical_cast<string>( date );
    return !in_spinup;
}

//------------------------------------------------------------------------------
// documentation is inherited
void CSVFluxPoolVisitor::visit( Core* c ) {
    run_name = c->getRun_name();
    core = c;
}

//------------------------------------------------------------------------------
/*! \brief Print the sources, and associated fractions, of a SimpleNBox pool
 */
void CSVFluxPoolVisitor::print_pool(fluxpool x) {
    if(x.tracking) {
        csvFile << datestring << DELIMITER << x.name << DELIMITER << x.value(U_PGC) << DELIMITER << x.units();
        
        vector<string> sources = x.get_sources();
        for (auto &s: sources) {
            csvFile << DELIMITER << s << DELIMITER << x.get_fraction(s);
        }
        csvFile << endl;
    }
}

//------------------------------------------------------------------------------
// documentation is inherited
void CSVFluxPoolVisitor::visit( SimpleNbox* c ) {
    if( !core->outputEnabled( c->getComponentName() ) ) return;

    // The potentially tracked pools
    print_pool( c->atmos_c );
    print_pool( c->earth_c );
}

}
