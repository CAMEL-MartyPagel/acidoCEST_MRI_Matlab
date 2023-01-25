function[chemical_data]=update_parameters(parameters, chemical_data)

    %% Update chemical_data file based on parameter array
    for spinsystem_iterator=1:length(chemical_data.spinsystem_list)
         
        % Hz offset of each pool
        if (chemical_data.spinsystem_list(spinsystem_iterator).fit_Hz_offset == true)
            chemical_data.spinsystem_list(spinsystem_iterator).Hz_offset=parameters(1);
            parameters(1)=[];
        end
        
        % R1
        if (chemical_data.spinsystem_list(spinsystem_iterator).fit_R1 == true)
            chemical_data.spinsystem_list(spinsystem_iterator).R1=parameters(1);
            parameters(1)=[];
        end
        
        % R2
        if (chemical_data.spinsystem_list(spinsystem_iterator).fit_R2 == true)
            chemical_data.spinsystem_list(spinsystem_iterator).R2=parameters(1);
            parameters(1)=[];
        end   
    end
    
    %% Update 1st Z scale factor (for baseline correction)
    if (chemical_data.fit_Z_scale_factor == true)
        chemical_data.Z_scale_factor=parameters(1);
        parameters(1)=[];
    end
    
    %% Update B1 Correction parameter
    if (chemical_data.fit_B1_correction==true)
        chemical_data.B1_correction=parameters(1);
        parameters(1)=[];
    end

    %% Update Experiment Level Data
    for experiment_iterator=1:length(chemical_data.experiment_list)
        
        % Update 2nd Z scale factor (for baseline correction)
        if (chemical_data.experiment_list(experiment_iterator).fit_Z_scale_factor == true)
            chemical_data.experiment_list(experiment_iterator).Z_scale_factor=parameters(1);
            parameters(1)=[];
        end
        
        % pH
        if (chemical_data.experiment_list(experiment_iterator).fit_pH == true)
            chemical_data.experiment_list(experiment_iterator).pH=parameters(1);
            parameters(1)=[];
        end
        
        % Concentration
        if (chemical_data.experiment_list(experiment_iterator).fit_concentration == true)
            chemical_data.experiment_list(experiment_iterator).concentration=parameters(1);
            parameters(1)=[];
        end
        
        %% Using information above, update simulated parameters
        
        % Use the daddy spin system list as initial setup for simulated Z spectrum
        chemical_data.experiment_list(experiment_iterator).simulated_spinsystem_list=chemical_data.spinsystem_list;
        
        % Use the daddy waveform list as initial guess. Then update it with the B1 correction (Note that B1_correction is set to 1 if you aren't fitting it and you don't have B1 map)
        chemical_data.simulated_waveform_list = chemical_data.waveform_list;
        chemical_data.simulated_waveform_list(1,2) = chemical_data.waveform_list(1,2) .* chemical_data.B1_correction;
        
        for spinsystem_iterator=1:length(chemical_data.experiment_list(experiment_iterator).simulated_spinsystem_list)
            
            % Update pH value
            kw = 13.6; %Kyle is using this number for kw in the body (unbuffered). It is cited as this in the following book: Cardiopulmonary Bypass: Principles and Practice pg. 158
            if (chemical_data.experiment_list(experiment_iterator).fit_pH == true)
                if(chemical_data.experiment_list(experiment_iterator).simulated_spinsystem_list(spinsystem_iterator).exogenous_agent==true)
                    chemical_data.experiment_list(experiment_iterator).simulated_spinsystem_list(spinsystem_iterator).exchange_rate=1e7*10^(-1*(kw-chemical_data.experiment_list(experiment_iterator).pH))*chemical_data.spinsystem_list(spinsystem_iterator).exchange_rate_base_catalyzed+chemical_data.spinsystem_list(spinsystem_iterator).exchange_rate_naught;
                else
                    chemical_data.spinsystem_list(spinsystem_iterator).exchange_rate;
                end
            end
        
            % Update Concentration for post-injection Z signal - Why are numbers multiplied together?
            if (spinsystem_iterator > 1)
                if (chemical_data.experiment_list(experiment_iterator).simulated_spinsystem_list(spinsystem_iterator).exogenous_agent==true)
                    chemical_data.experiment_list(experiment_iterator).simulated_spinsystem_list(spinsystem_iterator).concentration=chemical_data.experiment_list(experiment_iterator).concentration*chemical_data.spinsystem_list(spinsystem_iterator).concentration;
                else
                    chemical_data.experiment_list(experiment_iterator).simulated_spinsystem_list(spinsystem_iterator).concentration=chemical_data.spinsystem_list(spinsystem_iterator).concentration;
                end
            end
        end
    end
end

