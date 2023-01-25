function[parameters, lower_bound, upper_bound, typical_parameters, chemical_data] = initialize_parameters(chemical_data)
    %% Initialize parameter lists
    parameters=[];
    upper_bound=[];
    lower_bound=[];
    typical_parameters=[];
    
    %% Initialize parameters for each pool of iopamidol.
    for spinsystem_iterator=1:length(chemical_data.spinsystem_list)
        
        % Initialize Hz offset guess while adjusting for B0 shift
        if (chemical_data.spinsystem_list(spinsystem_iterator).fit_Hz_offset == true)
            parameters(length(parameters)+1)=chemical_data.spinsystem_list(spinsystem_iterator).Hz_offset + chemical_data.B0_shift;
            lower_bound(length(lower_bound)+1)=chemical_data.spinsystem_list(spinsystem_iterator).Hz_offset + chemical_data.B0_shift - 377;%modified by TLi 20200902, to limit the fitting of Hz offset to 0.2 ppm both sides
            upper_bound(length(upper_bound)+1)=chemical_data.spinsystem_list(spinsystem_iterator).Hz_offset + chemical_data.B0_shift + 377;
            typical_parameters(length(typical_parameters)+1)=1e5;
        end
        
        % Initialize R1 guess
        if (chemical_data.spinsystem_list(spinsystem_iterator).fit_R1 == true)
            parameters(length(parameters)+1)=chemical_data.spinsystem_list(spinsystem_iterator).R1;
            lower_bound(length(lower_bound)+1)=.2;
            upper_bound(length(upper_bound)+1)=10;
            typical_parameters(length(typical_parameters)+1)=2;
        end
        
        % Initialize R2 guess
        if (chemical_data.spinsystem_list(spinsystem_iterator).fit_R2 == true)
            parameters(length(parameters)+1)=chemical_data.spinsystem_list(spinsystem_iterator).R2;
            lower_bound(length(lower_bound)+1)=.2;
            upper_bound(length(upper_bound)+1)=40;
            typical_parameters(length(typical_parameters)+1)=20;
        end   
    end
    
    %% Initialize first Z scale factor (which is used for baseline correction)
    if (chemical_data.fit_Z_scale_factor == true)
        parameters(length(parameters)+1) = chemical_data.Z_scale_factor;
        lower_bound(length(lower_bound)+1) = 0;
        upper_bound(length(upper_bound)+1) = 10;
        typical_parameters(length(typical_parameters)+1) = 1;
    end
    
    %% Initialize Power Scale Factor
    if (chemical_data.fit_B1_correction == true)
        parameters(length(parameters)+1) = chemical_data.B1_correction;
        lower_bound(length(lower_bound)+1) = 0.5;
        upper_bound(length(upper_bound)+1) = 1.5;
        typical_parameters(length(typical_parameters)+1) = 1;
    end
    
    %% Initialize parameters for both pH and concentration - Note that these parameters are not fit for the pre-injection scan.
    for experiment_iterator=1:length(chemical_data.experiment_list)
        
        % Initialize second Z scale factor (which is used for baseline correction)
        if (chemical_data.experiment_list(experiment_iterator).fit_Z_scale_factor == true)
            parameters(length(parameters)+1) = chemical_data.experiment_list(experiment_iterator).Z_scale_factor;
            lower_bound(length(lower_bound)+1) = 0.5;
            upper_bound(length(upper_bound)+1) = 2;
            typical_parameters(length(typical_parameters)+1) = 1;
        end
        
        % Initialize pH guess
        if (chemical_data.experiment_list(experiment_iterator).fit_pH == true)
            parameters(length(parameters)+1)=chemical_data.experiment_list(experiment_iterator).pH;
            lower_bound(length(lower_bound)+1)=5;
            upper_bound(length(upper_bound)+1)=8;
            typical_parameters(length(typical_parameters)+1)=7.1;
        end
        
        % Initialize concentration guess
        if (chemical_data.experiment_list(experiment_iterator).fit_concentration == true)
            parameters(length(parameters)+1)=chemical_data.experiment_list(experiment_iterator).concentration;
            lower_bound(length(lower_bound)+1)=0;
            upper_bound(length(upper_bound)+1)=0.1;
            typical_parameters(length(typical_parameters)+1)=0.01;
        end      
    end
end

