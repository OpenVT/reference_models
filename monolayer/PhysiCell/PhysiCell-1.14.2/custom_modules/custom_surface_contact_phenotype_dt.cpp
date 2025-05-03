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

void create_cell_types( void )
{
	// set the random seed 
	// SeedRandom( parameters.ints("random_seed") );  
	
	/* 
	   Put any modifications to default cell definition here if you 
	   want to have "inherited" by other cell types. 
	   
	   This is a good place to set default functions. 
	*/ 
	
	initialize_default_cell_definition(); 
	cell_defaults.phenotype.secretion.sync_to_microenvironment( &microenvironment ); 
	
	cell_defaults.functions.volume_update_function = standard_volume_update_function;
	cell_defaults.functions.update_velocity = standard_update_cell_velocity;

	cell_defaults.functions.update_migration_bias = NULL; 
	cell_defaults.functions.update_phenotype = NULL; // update_cell_and_death_parameters_O2_based; 
	cell_defaults.functions.custom_cell_rule = NULL; 
	cell_defaults.functions.contact_function = NULL; 
    cell_defaults.functions.cell_division_function = NULL; 
	
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
	
	cell_defaults.functions.update_phenotype = phenotype_function; 
	// cell_defaults.functions.custom_cell_rule = custom_function; 
	// cell_defaults.functions.contact_function = contact_function; 
    cell_defaults.functions.cell_division_function = custom_division_function; 
    // cell_defaults.functions.volume_update_function = double_volume_update_function; 
	
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

std::vector<std::string> my_coloring_function( Cell* pCell )
{ return paint_by_number_cell_coloring(pCell); }

void phenotype_function( Cell* pCell, Phenotype& phenotype, double dt )
{ 
    // std::cout << __FUNCTION__ << ": " << PhysiCell_globals.current_time << ": cell ID= " << pCell->ID << std::endl;

    double r1 = phenotype.geometry.radius;
    double sum_chords = 0.0;
	for( int idx=0; idx<pCell->state.neighbors.size(); idx++ )
	{
		Cell* pC = pCell->state.neighbors[idx]; 

        // compute chord of intersection (if any)
        // radii of cells
        double r2 = pC->phenotype.geometry.radius;
        // centers of cells
        double x1 = (*pCell).position[0];
        double y1 = (*pCell).position[1];
        double x2 = (*pC).position[0];
        double y2 = (*pC).position[1];
        double xdiff = x1-x2;
        double ydiff = y1-y2;
        double d = sqrt(xdiff*xdiff + ydiff*ydiff);
        if (d < r1+r2)
        {
            // std::cout << "cell " << pCell->ID << " intersects cell " << pC->ID << std::endl;
            // std::cout << "x1,y1 " << x1 << ", " << y1 << std::endl;
            // std::cout << "x2,y2 " << x2 << ", " << y2 << std::endl;
            // std::cout << "  r1,r2 " << r1 << ", " << r2 << std::endl;
            // std::cout << "cell " << pCell->ID << " intersects cell " << pC->ID << std::endl;

            double a = (r1*r1 - r2*r2 + d*d) / (2 * d);
            double h = sqrt(r1*r1 - a*a);
            double x3 = x1 + a * (x2 - x1) / d;
            double y3 = y1 + a * (y2 - y1) / d;

            double p1x = x3 + h * (y2 - y1) / d;
            double p1y = y3 - h * (x2 - x1) / d;
            double p2x = x3 - h * (y2 - y1) / d;
            double p2y = y3 + h * (x2 - x1) / d;

            // std::cout << "p1= " << p1x <<", "<< p1y << std::endl;
            // std::cout << "p2= " << p2x <<", "<< p2y << std::endl;

            xdiff = p1x-p2x;
            ydiff = p1y-p2y;
            d = sqrt(xdiff*xdiff + ydiff*ydiff);
            // std::cout << "dist= " << d << std::endl;
            sum_chords += d;
        }
    }
    double pct_surface_contact = sum_chords/(6.28319*r1);
    pCell->custom_data["pct_surface_contact"] = pct_surface_contact;
    // std::cout << "-------- sum_chords = " << sum_chords << ",  pct_surface_contact= " << pct_surface_contact << std::endl;
    // vs.  -- is this logic really correct - in void Cell::remove_self_from_all_neighbors()
    // for( int j = 0 ; j < pCell->state.neighbors.size(); j++ )
	// {
	//  	Cell* pN = pCell->state.neighbors[j]; 
    //     auto SearchResult = std::find( 
    //         pN->state.neighbors.begin(),pN->state.neighbors.end(),pCell );  		

    //     // if pC is indeed found, remove it  
    //     // erase pC from neighbors 
    //     if( SearchResult != pN->state.neighbors.end() )
    //     {
    //         // if the target is found, set the appropriate rate 
    //         pN->state.neighbors.erase( SearchResult ); 
    //     }
    //     else
    //     { /* future error message */  }
	// }
    return; 
}

void custom_function( Cell* pCell, Phenotype& phenotype , double dt )
{ 
    std::cout << __FUNCTION__ << ": " << PhysiCell_globals.current_time << ": cell ID= " << pCell->ID << std::endl;
    return; 
} 

void contact_function( Cell* pMe, Phenotype& phenoMe , Cell* pOther, Phenotype& phenoOther , double dt )
{ return; } 

// void double_volume_update_function( Cell* pCell, Phenotype& phenotype , double dt )
// {
//     // Warning! Do not use get_total_volume!
//     // Use (some_cell).phenotype.volume.total instead!
//     // pCell->set_total_volume( pCell->phenotype.volume.total + pCell->phenotype.volume.total * 0.1);
//     // if ( get_single_signal( pCell, "pressure" ) < 3.0 )
//     // {
//     //     pCell->set_total_volume( pCell->phenotype.volume.total + pCell->phenotype.volume.total * pCell->custom_data["growth_rate"]);
//     // }
//     pCell->set_total_volume( pCell->phenotype.volume.total + pCell->phenotype.volume.total * pCell->custom_data["growth_rate"]);

//     // if (pCell->phenotype.volume.total > 1047)    //rwh: hard-coded; fix!
//     if (pCell->phenotype.volume.total > NormalRandom(2.0, 0.25) * 523.6)    //rwh: hard-coded; fix!
//     // if (pCell->phenotype.volume.total > NormalRandom(2.0, 1.0) * 523.6)    //rwh: hard-coded; fix!
//     {
//         // std::cout << "------- " << __FUNCTION__ << ":  ID= " << pCell->ID <<":  volume.total= " << pCell->phenotype.volume.total << std::endl;
//         pCell->flag_for_division();
//     }
// }

// the only reason for this fn is to exit the sim at 10K cells
void custom_division_function( Cell* pCell1, Cell* pCell2 )
{ 
    static int monolayer_max_cells = 10000;
    // static int idx_default = find_cell_definition_index("default");
    // static int idx_ctype1 = find_cell_definition_index("ctype1");
    // std::cout << __FUNCTION__ << ": " << PhysiCell_globals.current_time << ": cell IDs= " << pCell1->ID << ", " << pCell2->ID << std::endl;

    // // Asymmetric division
    // if (UniformRandom() < 0.5)
    // {
    //     pCell2->convert_to_cell_definition( *cell_definitions_by_index[idx_default] ); 
    // }
    // else
    // {
    //     pCell2->convert_to_cell_definition( *cell_definitions_by_index[idx_ctype1] ); 
    // }

	char filename[1024];
    static std::vector<std::string> (*cell_coloring_function)(Cell*) = my_coloring_function;
    static std::string (*substrate_coloring_function)(double, double, double) = paint_by_density_percentage;

    int ncells = (*all_cells).size();
    if ( ncells > monolayer_max_cells )
    {
        std::cout << "-------- # cells: " << ncells << std::endl;
        sprintf( filename , "%s/output%08u" , PhysiCell_settings.folder.c_str(),  PhysiCell_globals.full_output_index ); 
        // sprintf( filename , "%s/final" , PhysiCell_settings.folder.c_str() ); 
        save_PhysiCell_to_MultiCellDS_v2( filename , microenvironment , PhysiCell_globals.current_time ); 
        
        sprintf( filename , "%s/snapshot%08u.svg" , PhysiCell_settings.folder.c_str() , PhysiCell_globals.SVG_output_index );
        // sprintf( filename , "%s/final.svg" , PhysiCell_settings.folder.c_str() );
        SVG_plot(filename, microenvironment, 0.0, PhysiCell_globals.current_time, cell_coloring_function, substrate_coloring_function);

        // timer 
        std::cout << std::endl << "Total simulation runtime: " << std::endl; 
        BioFVM::display_stopwatch_value( std::cout , BioFVM::runtime_stopwatch_value() ); 

        std::cout << std::endl; 
        exit(-1);
    }

    return; 
}