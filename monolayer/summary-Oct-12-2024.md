From: Roman Vetter
Date: Saturday, 12 October 2024 at 6:14 AM
To: Randy Heiland, Lutz Brusch, James Osborne, Boris Aguilar, Yi Jiang
Subject: [EXT] OpenVT: Monolayer model specification, manuscript & git

Dear Monolayer Growth modelers,

yesterday Randy and I had a good discussion about the Monolayer Growth reference model. I want to summarize here what we have discussed in the model team and also among all meeting attendees, and propose the next steps. Feel free to object to any of these - we were discussing only among 2-3 team members (2 for most of the time).

Have I missed anyone in the list of recipients?
 

# Model scope

We agreed that we need to avoid too much modeling complexity, too many model variants, and too high ambitions regarding new biology/physics. As it is currently formulated, we have already several contact inhibition types, each with its own parameter, varying degrees of relaxation (damping), a parameter sensitivity study, different levels of noise in growth and division, etc. - it's just too much. To get anywhere at all, we should rather focus on a simple setup that is compatible with as many implementations as possible, rather than trying to produce new biological or physical insight. This implies the following:

1) We drop the idea of measuring the fractal dimension of the boundary with the basic reference model for now. This would be a nice project, but turned out to be nontrivial computationally so far. Most frameworks either have not been able to produce a fractal boundary, or yielded a fractal dimension that is not comparable to the one reported from experiments. Simulating this would be an interesting (potentially bilateral) follow-up project with a different creative focus.

2) For the basic model, we would have randomly oriented cell divisions to maximize compatibility with implementations in which cell shape anisotropy is not an inherent property.

3) Moreover, to even further maximize compatibility, we think that randomness in growth and proliferation could be excluded for the baseline case. All cells grow at the same rate and divide when they have reached exactly twice the initial A_0. This will rule out any advanced/realistic quantification such as Lewis/Aboav-Weaire laws, crystalline order, topological defects etc., and it will keep the cells in sync up to the effects of contact inhibition. In the analysis category 3, we would only measure the fraction of quiescent cells over time, and the final cell neighbor number distribution. Keeping it simple and broadly applicable.
 

# Model parameters

Since we want to report times in units of uninhibited cell cycle length, and lengths in units of cell radius, we are down to effectively only 2-3 shared model parameters (which is a good thing in my view):

1) 0<=β<=1 for contact inhibition type 1 (pressure-based; grow cell only if A/A_0 > β)

2) 0<=γ<=1 for contact inhibition type 2 (contact fraction-based; grow cell only if fraction of perimeter in contact with other cells < γ)

3) viscous damping (which may apply only to some frameworks, but not all, as I found out yesterday)

There are certainly other parameters such as cell stiffness & adhesiveness in some frameworks, and that will affect the boundary roughness etc., but this will be highly implementation-dependent. The goal would be to report in the paper what parameters each framework needs to have set to what values to get quantitative agreement.

Therefore I would suggest to perform three basic simulations per implementation: One with β=0.8 for type 1, one with γ=0.5 for type 2, and one without contact inhibition (type 3). We would simulate until N=10^4 cells and have a first quantitative comparison based on that. Then we can see next time whether we want to increase N, change parameters, add randomness, etc.


# Git repository

The organizers would like us to put all relevant code/data/info into a git repository, one for each reference model. The goal appears to be to make this public and accessible together with the paper, so this would need to be kept quite tidy. I have set one up for the Monolayer Growth model at https://gitlab.com/rvet/monolayergrowth . Note that the folders "implementations" and "results" are meant to have subfolders, one for each framework. Feel free to push your code/data. So far I have only added the basic model schema and some literature, as apparently not all of us have access to all of it.

Please let me know whom to add to this repository. I'm no git expert - apparently either a gitlab.com user can be added as project member, or I can hand out access tokens (?)...

 
# Manuscript

The different model teams are now starting to prepare manuscript drafts, one for each reference model. We decided that Google Docs would be the best platform to get this started, and I set it up at https://docs.google.com/document/d/1k3WwodgGDGK5TSk_L2UmAf0ClrpXRP52Y-N3y6cXezs/edit . Please let me know if anybody else needs access, and please feel free to fill in things as you see fit.
