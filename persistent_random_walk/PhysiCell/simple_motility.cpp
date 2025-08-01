
// g++-14 simple_motility.cpp -O3 -fomit-frame-pointer -fopenmp -m64 -std=c++11 -o simple_motility

#include <iostream>
#include <random>
#include <omp.h>

thread_local std::mt19937_64 physicell_PRNG_generator; 
thread_local bool local_pnrg_setup_done = false; 

unsigned int physicell_random_seed = 0; 
std::vector<unsigned int> physicell_random_seeds; 

std::vector<double> cell_xpos; 
std::vector<double> cell_ypos; 

std::vector<double> position = {50.,0.,0.};
std::vector<double> velocity = {0.1,0.,0.};
std::vector<double> previous_velocity = {0.0,0.,0.};

double UniformRandom( void )
{
	thread_local std::uniform_real_distribution<double> distribution(0.0,1.0);
	if( local_pnrg_setup_done == false )
	{
		// get my thread number 
		int i = omp_get_thread_num(); 
        // std::cout << "UniformRandom():  i (thread #) = " << i << std::endl;  // = 0
    	// physicell_PRNG_generator.seed( physicell_random_seeds[i] ); 
        int myseed = 42;
        std::cout << "UniformRandom():  myseed= " << myseed << std::endl;
    	physicell_PRNG_generator.seed( myseed ); 
		local_pnrg_setup_done = true; 
/*
		#pragma omp critical 
		{
		std::cout << "thread: " << i 
		<< " seed: " << physicell_random_seeds[i]  << std::endl; 
		std::cout << "\t first call: " << distribution(physicell_PRNG_generator) << std::endl; 
		}
*/
	}
    return distribution(physicell_PRNG_generator);

	// helpful info: https://stackoverflow.com/a/29710970
/*

	static std::uniform_real_distribution<double> distribution(0.0,1.0); 
	double out;
	out = distribution(physicell_PRNG_generator);
	return out; 
*/	
}

// In P*_utilities.cpp
std::vector<double> UniformOnUnitCircle( void )
{
	std::vector<double> output = {0,0,0}; 

	static long double two_pi = 6.283185307179586476925286766559;  
	                       
	long double theta = UniformRandom(); //  BioFVM::uniform_random();
	theta *= two_pi; // Choose theta uniformly distributed on [0, 2*pi).

	output[0] = cos(theta); 
	output[1] = sin(theta); // (cos(t) , sin(t) , 0 )

	return output; 
}

// BioFVM_vector.cpp
void normalize( std::vector<double>* v )
{
 double norm = 1e-32; 

 for( unsigned int i=0; i < v->size(); i++ )
 { norm += ( (*v)[i] * (*v)[i] ); }
 norm = sqrt( norm ); 

 for( unsigned int i=0; i < v->size(); i++ )
 { (*v)[i] /=  norm ; }

 // If the norm is small, normalizing doens't make sense. 
 // Just set the entire vector to zero. 
 static bool I_warned_you = false; 
 if( norm <= 1e-16 )
 { 
  if( I_warned_you == false )
  {
   std::cout << "Warning and FYI: Very small vectors are normalized to 0 vector" << std::endl << std::endl; 
   I_warned_you = true; 
  }

  for( unsigned int i=0; i < v->size(); i++ )
  { (*v)[i] = 0.0; }
 }

 return; 
}

// BioFVM_vector.cpp
void axpy( std::vector<double>* y, const double& a , const std::vector<double>& x )
{
 for( unsigned int i=0; i < (*y).size() ; i++ )
 {
  (*y)[i] += a * x[i] ; 
 }
 return ; 
}

// in core/P*_cell.cpp
//void Cell::update_motility_vector( double dt_ )
void update_motility_vector( double dt_ )
{
    std::vector<double> randvec(3,0.0);
    static double migration_bias = 0.5;
    static double migration_speed = 0.1;
    static double persistence_time = 0.0;

    // if( phenotype.motility.is_motile == false )
    // {
    //         phenotype.motility.motility_vector.assign( 3, 0.0 );
    //         return;
    // }
    // if( UniformRandom() < dt_ / phenotype.motility.persistence_time || phenotype.motility.persistence_time < dt_ )
    if( UniformRandom() < dt_ / persistence_time || persistence_time < dt_ )
    {
            // if( phenotype.motility.restrict_to_2D == true )
            { randvec = UniformOnUnitCircle(); 
              std::cout << "randvec= " << randvec[0] << ", " << randvec[1] << std::endl; 
            }
            // else
            // { randvec = UniformOnUnitSphere(); }
            // // if the update_bias_vector function is set, use it
            // if( functions.update_migration_bias )
            // {
            //         functions.update_migration_bias( this,phenotype,dt_ );
            // }
        
        // recall in custom.cpp, we did:
        // std::vector<double> ctype1_direction {1.0, 0.0, 0.0};
        // cell_defaults.phenotype.motility.migration_bias_direction = ctype1_direction;	

            // phenotype.motility.motility_vector = phenotype.motility.migration_bias_direction; // motility = bias_vector
            // motility_vector = {1.,0.,0.};
            std::vector<double> motility_vector {1.0, 0.0, 0.0};   // rwh: do we really reset?
            // phenotype.motility.motility_vector *= phenotype.motility.migration_bias; // motility = bias*bias_vector
            // motility_vector *= migration_bias; // motility = bias*bias_vector
            motility_vector[0] *= migration_bias; // motility = bias*bias_vector
            motility_vector[1] *= migration_bias; // motility = bias*bias_vector
            // double one_minus_bias = 1.0 - phenotype.motility.migration_bias;
            double one_minus_bias = 1.0 - migration_bias;
            // axpy( &(phenotype.motility.motility_vector), one_minus_bias, randvec ); // motility = (1-bias)*randvec + bias*bias_vector
            axpy( &(motility_vector), one_minus_bias, randvec ); // motility = (1-bias)*randvec + bias*bias_vector
            // for (size_t i = 0; i < motility_vector.size(); ++i) 
            // {
            //     randvec[i] += one_minus_bias * motility_vector[i];
            // }
            // normalize( &(phenotype.motility.motility_vector) );
            normalize( &(motility_vector) );
            // phenotype.motility.motility_vector *= phenotype.motility.migration_speed;
            // motility_vector *= migration_speed;
            motility_vector[0] *= migration_speed;
            motility_vector[1] *= migration_speed;

            // cell_xpos.push_back(motility_vector[0]);
            // cell_ypos.push_back(motility_vector[1]);

            std::cout << "motility_vector= "<< motility_vector[0] << ", " << motility_vector[1] << std::endl;
    }
    return;
}

// void Cell::update_position( double dt )
void update_position( double dt )
{
	// BioFVM Basic_Agent::update_position(dt) returns without doing anything. 
	// So we remove this to avoid any future surprises. 
	// 
	// Basic_Agent::update_position(dt);
		
	// use Adams-Bashforth 
	static double d1; 
	static double d2; 
	static bool constants_defined = false; 
	if( constants_defined == false )
	{
		d1 = dt; 
		d1 *= 1.5; 
		d2 = dt; 
		d2 *= -0.5; 
		constants_defined = true; 
	}
	
	// new AUgust 2017
	// if( default_microenvironment_options.simulate_2D == true )
	{ velocity[2] = 0.0; }
	
	// std::vector<double> old_position(position); 
	axpy( &position , d1 , velocity );  
	axpy( &position , d2 , previous_velocity );  
	// overwrite previous_velocity for future use 
	// if(sqrt(dist(old_position, position))>3* phenotype.geometry.radius)
		// std::cout<<sqrt(dist(old_position, position))<<"old_position: "<<old_position<<", new position: "<< position<<", velocity: "<<velocity<<", previous_velocity: "<< previous_velocity<<std::endl;
	
	previous_velocity = velocity; 
	
	velocity[0]=0; velocity[1]=0; velocity[2]=0;
	// if(get_container()->underlying_mesh.is_position_valid(position[0],position[1],position[2]))
	// {
	// 	updated_current_mechanics_voxel_index=get_container()->underlying_mesh.nearest_voxel_index( position );
	// }
	// else
	// {
	// 	updated_current_mechanics_voxel_index=-1;
		
	// 	is_out_of_domain = true; 
	// 	is_active = false; 
	// 	is_movable = false; 
	// }
	return; 
}


int main()
{
    omp_set_num_threads(1);
    for (int idx=0; idx<5; idx++)
    {
        update_motility_vector( 0.1 );
        // std::cout <<"motility vector= "<<cell_xpos[idx]<<","<<cell_ypos[idx]<<std::endl;
        update_position( 0.1 );
        std::cout <<"cell pos= "<<position[0]<<","<<position[1]<<std::endl;
    }

    // std::cout <<"\n-------------"<<std::endl;
    // for (int idx=0; idx<cell_xpos.size(); idx++)
    // {
    //     std::cout <<"motility vector= "<<cell_xpos[idx]<<","<<cell_ypos[idx]<<std::endl;
    //     // std::cout <<"cell pos= "<<position[idx]<<","<<position[idx]<<std::endl;
    // }
}
