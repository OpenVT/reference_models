/*
###############################################################################
# If you use PhysiCell in your project, please cite PhysiCell and the version #
# number, such as below:                                                      #
#                                                                             #
# We implemented and solved the model using PhysiCell (Version x.y.z) [1].    #
#                                                                             #
# [1] A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellu-  #
#     lar Systems, PLoS Comput. Biol. 14(2): e1005991, 2018                   #
#     DOI: 10.1371/journal.pcbi.1005991                                       #
#                                                                             #
# See VERSION.txt or call get_PhysiCell_version() to get the current version  #
#     x.y.z. Call display_citations() to get detailed information on all cite-#
#     able software used in your PhysiCell application.                       #
#                                                                             #
# Because PhysiCell extensively uses BioFVM, we suggest you also cite BioFVM  #
#     as below:                                                               #
#                                                                             #
# We implemented and solved the model using PhysiCell (Version x.y.z) [1],    #
# with BioFVM [2] to solve the transport equations.                           #
#                                                                             #
# [1] A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellu-  #
#     lar Systems, PLoS Comput. Biol. 14(2): e1005991, 2018                   #
#     DOI: 10.1371/journal.pcbi.1005991                                       #
#                                                                             #
# [2] A Ghaffarizadeh, SH Friedman, and P Macklin, BioFVM: an efficient para- #
#     llelized diffusive transport solver for 3-D biological simulations,     #
#     Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730  #
#                                                                             #
###############################################################################
#                                                                             #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)     #
#                                                                             #
# Copyright (c) 2015-2021, Paul Macklin and the PhysiCell Project             #
# All rights reserved.                                                        #
#                                                                             #
# Redistribution and use in source and binary forms, with or without          #
# modification, are permitted provided that the following conditions are met: #
#                                                                             #
# 1. Redistributions of source code must retain the above copyright notice,   #
# this list of conditions and the following disclaimer.                       #
#                                                                             #
# 2. Redistributions in binary form must reproduce the above copyright        #
# notice, this list of conditions and the following disclaimer in the         #
# documentation and/or other materials provided with the distribution.        #
#                                                                             #
# 3. Neither the name of the copyright holder nor the names of its            #
# contributors may be used to endorse or promote products derived from this   #
# software without specific prior written permission.                         #
#                                                                             #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" #
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   #
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE  #
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   #
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         #
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF        #
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    #
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN     #
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)     #
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE  #
# POSSIBILITY OF SUCH DAMAGE.                                                 #
#                                                                             #
###############################################################################
*/

#include "./custom.h"

#include <set>
#include <utility>

void create_cell_types( void )
{
	// set the random seed 
	// SeedRandom( parameters.ints("random_seed") );  
	// SeedRandom();    // uses  std::chrono::system_clock::now().time_since_epoch().count();
	
	/* 
	   Put any modifications to default cell definition here if you 
	   want to have "inherited" by other cell types. 
	   
	   This is a good place to set default functions. 
	*/ 
	
	initialize_default_cell_definition(); 
	cell_defaults.phenotype.secretion.sync_to_microenvironment( &microenvironment ); 
	
	cell_defaults.functions.volume_update_function = standard_volume_update_function;
	// cell_defaults.functions.volume_update_function = custom_volume_update_function;
	cell_defaults.functions.update_velocity = standard_update_cell_velocity;

	cell_defaults.functions.update_migration_bias = NULL; 
	cell_defaults.functions.update_phenotype = NULL; // update_cell_and_death_parameters_O2_based; 
	cell_defaults.functions.custom_cell_rule = NULL; 
	cell_defaults.functions.contact_function = NULL; 
	
	cell_defaults.functions.add_cell_basement_membrane_interactions = NULL; 
	cell_defaults.functions.calculate_distance_to_membrane = NULL; 
	
	/*
	   This parses the cell definitions in the XML config file. 
	*/
	
	initialize_cell_definitions_from_pugixml(); 

	/*
	   This builds the map of cell definitions and summarizes the setup. 
	*/
		
	build_cell_definitions_maps(); 

	/*
	   This intializes cell signal and response dictionaries 
	*/

	setup_signal_behavior_dictionaries(); 	

	/*
       Cell rule definitions 
	*/

	setup_cell_rules(); 

	/* 
	   Put any modifications to individual cell definitions here. 
	   
	   This is a good place to set custom functions. 
	*/ 
	
	// cell_defaults.functions.update_phenotype = phenotype_function; 
	cell_defaults.functions.update_phenotype = NULL; 

	// cell_defaults.functions.custom_cell_rule = custom_function; 
	cell_defaults.functions.custom_cell_rule = NULL; 
	// cell_defaults.functions.contact_function = contact_function; 

    // cell_defaults.functions.update_migration_bias = custom_cell_motility;  //rwh
    // std::vector<double> ctype1_direction {1.0, 0.0, 0.0};
    // cell_defaults.phenotype.motility.migration_bias_direction = ctype1_direction;	
	
	/*
	   This builds the map of cell definitions and summarizes the setup. 
	*/
		
	display_cell_definitions( std::cout ); 
	
	return; 
}

void setup_microenvironment( void )
{
	// set domain parameters 
	
	// put any custom code to set non-homogeneous initial conditions or 
	// extra Dirichlet nodes here. 
	
	// initialize BioFVM 
	
	initialize_microenvironment(); 	
	
	return; 
}

void setup_tissue( void )
{
	double Xmin = microenvironment.mesh.bounding_box[0]; 
	double Ymin = microenvironment.mesh.bounding_box[1]; 
	double Zmin = microenvironment.mesh.bounding_box[2]; 

	double Xmax = microenvironment.mesh.bounding_box[3]; 
	double Ymax = microenvironment.mesh.bounding_box[4]; 
	double Zmax = microenvironment.mesh.bounding_box[5]; 
	
	if( default_microenvironment_options.simulate_2D == true )
	{
		Zmin = 0.0; 
		Zmax = 0.0; 
	}
	
	double Xrange = Xmax - Xmin; 
	double Yrange = Ymax - Ymin; 
	double Zrange = Zmax - Zmin; 
	
	// create some of each type of cell 
	
	Cell* pC;
	
	for( int k=0; k < cell_definitions_by_index.size() ; k++ )
	{
		Cell_Definition* pCD = cell_definitions_by_index[k]; 
		std::cout << "Placing cells of type " << pCD->name << " ... " << std::endl; 
		for( int n = 0 ; n < parameters.ints("number_of_cells") ; n++ )
		{
			std::vector<double> position = {0,0,0}; 
			position[0] = Xmin + UniformRandom()*Xrange; 
			position[1] = Ymin + UniformRandom()*Yrange; 
			position[2] = Zmin + UniformRandom()*Zrange; 
			
			pC = create_cell( *pCD ); 
			pC->assign_position( position );
		}
	}
	std::cout << std::endl; 
	
	// load cells from your CSV file (if enabled)
	load_cells_from_pugixml();
	set_parameters_from_distributions();
	
	return; 
}

// std::vector<double> ctype1_direction {1.0, 0.0, 0.0};

// void custom_cell_motility( Cell* pCell, Phenotype& phenotype, double dt )
// {
//     // migration_bias_direction = {1.,0.,0.};

//     phenotype.motility.migration_bias_direction = ctype1_direction;	
// 		// normalize( &( phenotype.motility.migration_bias_direction ) );			
// 	return; 
// }

std::vector<std::string> my_coloring_function( Cell* pCell )
{ return paint_by_number_cell_coloring(pCell); }

void phenotype_function( Cell* pCell, Phenotype& phenotype, double dt )
{ 
    // return;

    static double vol_max = 2494;
    std::cout << PhysiCell_globals.current_time  << "  ---- phenotype_function: " << pCell->ID << " has vol= "<<pCell->phenotype.volume.total << std::endl;
    // if (pCell->phenotype.volume.total < 1000)
    // {
    //     std::cout << " -------------------------------- "<< std::endl;
    //     // vol2 = pC->phenotype.volume.total * 1.05; 
    //     // pC->phenotype.volume.total * 1.05; 
    //     // pCell->phenotype.volume.total = vol_max;
    //     // std::cout << "custom_function(): time= " << PhysiCell_globals.current_time << std::endl;
    //     std::cout << PhysiCell_globals.current_time  << "  ---- reset: "<<pCell->ID << " = " << vol_max << std::endl;
    //     pCell->set_total_volume( vol_max );
    // }

    // pCell->set_total_volume( 300 );
    for (int idx=0; idx< (*all_cells).size(); idx++)
    {
        Cell* pC = (*all_cells)[idx];
        // if (pC->phenotype.volume.total < 1000)
        if (pC->phenotype.volume.total < 2200)
        {
            // vol2 = pC->phenotype.volume.total * 1.05; 
            // pC->phenotype.volume.total * 1.05; 
            pC->set_total_volume( vol_max );
            // pC->set_total_volume( 2000 );
        }
        // std::cout << "pCell volume= " << vol1 << std::endl;
        // vol1 = pMe->phenotype.volume.total * 0.9;
    }
}

std::set<std::pair<int,Cell *>> nbrs = {};
std::set<std::pair<int,Cell *>> nbrs_prev = {};

//   mySet.insert(6); // {1, 2, 5, 6, 8, 9}
//   mySet.erase(2);  // {1, 5, 6, 8, 9}
//   // Search
//    if (mySet.find(5) != mySet.end()) {
//       std::cout << "Element 5 found" << std::endl;
//    }
//   // Iteration
//   std::cout << "Set elements: ";
//   for (int element : mySet) {
//     std::cout << element << " ";
//   }

void custom_function( Cell* pCell, Phenotype& phenotype , double dt )
{ 
    // static double vol_min = 300;
    static double vol_min = 1000;
    static double vol_max = 2494;
    static bool nbrs_empty;

    // ONLY called for ctype1 (not wall!)
    // std::cout << "  ---- custom_function: "  << PhysiCell_globals.current_time  << " ID= " << pCell->ID << " has vol= "<<pCell->phenotype.volume.total << std::endl;
    // if (nbrs.size() == 0)
    // {
    //     nbrs_empty = True;
    // }
    double vol1,vol2;

    pCell->set_total_volume( vol_max );
    nbrs.clear();
    for( int n=0; n < pCell->state.neighbors.size(); n++ )
	{
        // pCell->set_total_volume( 1500 );
        pCell->set_total_volume( 2494 );
        // vol1 = pCell->phenotype.volume.total;
        // std::cout << "pCell volume= " << vol1 << std::endl;
        // vol1 = pMe->phenotype.volume.total * 0.9;
        // if (nbrs_empty)

        Cell* pC = pCell->state.neighbors[n]; 
        std::pair<int, Cell *> id_pcell(pC->ID, pC);
        nbrs.insert(id_pcell);
        vol2 = pC->phenotype.volume.total;
        if (vol2 > vol_min)
        {
            vol2 = pC->phenotype.volume.total * 0.95;
            // pCell->set_total_volume( vol1 );
            pC->set_total_volume( vol2 );
        }
	}
    // std::cout << "nbrs: ";
    // for (auto x: nbrs) 
    //     std::cout << x.first << " --> " << x.second << std::endl;

    // // std::cout << std::endl;
    // if (nbrs_prev.size() == 0)
    // {
    //     nbrs_prev = nbrs;
    // }
    // else   // nbrs_prev exists
    // {
    //     std::cout << "nbrs_prev: ";
    //     for (auto x: nbrs_prev) 
    //         std::cout << x.first << " --> " << x.second << std::endl;
    //     for (auto x: nbrs_prev)  // for each x in nbrs_prev: is it in current nbrs
    //     {
    //         // std::cout << x << ",";
    //         auto it = nbrs.find(x);
    //         if (it == nbrs.end())  // x is NOT in nbrs --> increase/restore its volume
    //         {
    //             vol2 = (x.second)->phenotype.volume.total * 1.05;
    //             if (vol2 <= vol_max)
    //             {
    //                 (x.second)->set_total_volume( vol2 );
    //             }
    //         }
    //     }
    // }
    return; 
}

void contact_function( Cell* pMe, Phenotype& phenoMe , Cell* pOther, Phenotype& phenoOther , double dt )
{ 
    // pCell->set_total_volume( pCell->phenotype.volume.total + pCell->phenotype.volume.total * pCell->custom_data["growth_rate"]);
    return; 
} 