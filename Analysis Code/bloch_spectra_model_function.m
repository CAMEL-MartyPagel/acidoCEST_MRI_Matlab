function[chemical_data]=bloch_spectra_model_function(parameters, chemical_data)
    %model function to fit via nlinfit, this is actually what we need
    
    %i want to fit multiple samples at once, so I need to administrate fitting for each
    %i want to read in the data for each experiment, then fully run that experiment
    
    
    %update chemical info with new parameters from matlab's fitting script
    [chemical_data]=update_parameters(parameters, chemical_data);
    
    
    for experiment_number=1:length(chemical_data.experiment_list)
        
        %OK, now that our parameters are in place, build the bloch matrix
        if(~isfield(chemical_data,'lineshape_list'))
            chemical_data.lineshape_list=[];
        end
        
        %Incorporate old build_bloch_matrix into this function - Marty Pagel
        spinsystem_list=chemical_data.experiment_list(experiment_number).simulated_spinsystem_list;
        lineshape_list=chemical_data.lineshape_list;
        
        % Build everything in 2D arrays.
        %zero exchange matrix
        exchange_matrix=zeros(length(spinsystem_list));
        %do exchange matrix
        for spinsystem_iterator=2:length(spinsystem_list)
            
            %update exchange matrix
            %water to pool
            exchange_matrix(1,1)=exchange_matrix(1,1)-spinsystem_list(spinsystem_iterator).concentration/spinsystem_list(1).concentration*spinsystem_list(spinsystem_iterator).exchange_rate;
            exchange_matrix(spinsystem_iterator,1)=exchange_matrix(spinsystem_iterator,1)+spinsystem_list(spinsystem_iterator).concentration/spinsystem_list(1).concentration*spinsystem_list(spinsystem_iterator).exchange_rate;
            %pool to water
            exchange_matrix(spinsystem_iterator,spinsystem_iterator)=exchange_matrix(spinsystem_iterator,spinsystem_iterator)-spinsystem_list(spinsystem_iterator).exchange_rate;
            exchange_matrix(1,spinsystem_iterator)=exchange_matrix(1,spinsystem_iterator)+spinsystem_list(spinsystem_iterator).exchange_rate;
            
        end
        
        %do bloch matrices
        %zero precession matrix
        precession_matrix=zeros(3*length(spinsystem_list));
        for spinsystem_iterator=1:length(spinsystem_list)
            
            precession_single=zeros(3);
            %build single bloch matrix
            %T1,T2
            precession_single(1,1)=-spinsystem_list(spinsystem_iterator).R2;
            precession_single(2,2)=-spinsystem_list(spinsystem_iterator).R2;
            precession_single(3,3)=-spinsystem_list(spinsystem_iterator).R1;
            
            %precession, right handed coordinate system
            precession_single(2,1)=-spinsystem_list(spinsystem_iterator).Hz_offset;
            precession_single(1,2)=spinsystem_list(spinsystem_iterator).Hz_offset;
            
            precession_matrix(3*(spinsystem_iterator-1)+1:3*(spinsystem_iterator-1)+3,3*(spinsystem_iterator-1)+1:3*(spinsystem_iterator-1)+3)=precession_single;
        end
        
        bloch_matrix=precession_matrix+kron(exchange_matrix,eye(3));
        
        %slap on the lineshapes, there might be fat, protein and mt, among others
        if (~isempty(lineshape_list))
            for lineshape_iterator=1:length(lineshape_list)
                
                %T1 relaxation & exchange, and resize the matrix
                bloch_matrix(size(bloch_matrix,1)+1,size(bloch_matrix,2)+1)=-lineshape_list(lineshape_iterator).R1-lineshape_list(lineshape_iterator).exchange_rate;
                
                %exchange, only work on z axis
                bloch_matrix(3,size(bloch_matrix,2))=lineshape_list(lineshape_iterator).exchange_rate;
                bloch_matrix(3,3)=bloch_matrix(3,3)-lineshape_list(lineshape_iterator).exchange_rate*lineshape_list(lineshape_iterator).concentration;
                bloch_matrix(size(bloch_matrix,1),3)=lineshape_list(lineshape_iterator).exchange_rate*lineshape_list(lineshape_iterator).concentration;
            end
        end
        
        %inhomogenous part
        inhomogenous_part=zeros(3*length(spinsystem_list),1);
        
        for spinsystem_iterator=1:length(spinsystem_list)
            inhomogenous_part(3*spinsystem_iterator)=spinsystem_list(spinsystem_iterator).concentration.*spinsystem_list(spinsystem_iterator).R1;
        end
        
        for lineshape_iterator=1:length(lineshape_list)
            inhomogenous_part(3*length(spinsystem_list)+lineshape_iterator)=lineshape_list(lineshape_iterator).concentration.*lineshape_list(lineshape_iterator).R1;
        end
        bloch_matrix=horzcat(bloch_matrix,inhomogenous_part);
        bloch_matrix=vertcat(bloch_matrix,zeros(1,size(bloch_matrix,2)));
        
        %set up equilibrium magnetization
        %dump in dummy variable
        %Mnaught(3*length(chemical_data.experiment_list(experiment_number).spinsystem_list)+1)=1;
        %dump in info from simulated spin system
        Mnaught=[];
        for spinsystem_iterator=1:length(chemical_data.experiment_list(experiment_number).simulated_spinsystem_list)
            Mnaught(3*(spinsystem_iterator-1)+1)=0;
            Mnaught(3*(spinsystem_iterator-1)+2)=0;
            Mnaught(3*(spinsystem_iterator-1)+3)=chemical_data.experiment_list(experiment_number).simulated_spinsystem_list(spinsystem_iterator).concentration;
        end
        if(isfield(chemical_data,'lineshape_list'))
            Mnaught(end+1)=chemical_data.lineshape_list.concentration;
        end
        Mnaught(end+1)=1;
        
        Mnaught=reshape(Mnaught,[],1);
        
        %get propogator with the waveform
        Z_signal_array=[];
        Z_signal_array=zeros(1,length(chemical_data.experiment_list(experiment_number).Hz_offset));
        
        if(isfield(chemical_data,'lineshape_list'))
            %lineshape_func_handle=str2func(chemical_data.lineshape_list(1).lineshape_function);
            %lineshape_full=lineshape_func_handle(1./chemical_data.lineshape_list.R2,chemical_data.experiment_list(experiment_number).Hz_offset-chemical_data.lineshape_list.Hz_offset);
            lineshape_full=(1./chemical_data.lineshape_list.R2)./pi()./(1+((chemical_data.experiment_list(experiment_number).Hz_offset-chemical_data.lineshape_list.Hz_offset).*(1./chemical_data.lineshape_list.R2)).^2);
        end
        
        for	offset_iterator=1:length(chemical_data.experiment_list(experiment_number).Hz_offset)
            
            %read the waveform in time /  nutation rate / offset format
            waveform = chemical_data.simulated_waveform_list;
            %push the entire waveform off resonance
            waveform(:,3)=chemical_data.experiment_list(experiment_number).Hz_offset(offset_iterator);
            
            if(isfield(chemical_data,'lineshape_list'))
                lineshape=ones(size(waveform,1),1).*lineshape_full(offset_iterator);
            end
            
            if(isfield(chemical_data,'lineshape_list'))
                    [bloch_propogator]=bloch_evolve(bloch_matrix,waveform,lineshape);
                    
                    %if there is linear encoding, or there is a delay after presat
                    %(steady state or whatever) get the propogator for no presat as
                    %well. i don't know how to make this pretty, please don't
                    %laugh at my bad code
                    if(isfield(chemical_data.experiment_list(experiment_number),'linear_encoding_delay'))%Question from TLi: what is the lineshape_list and linear_encoding_delay?
                        [bloch_delay_propogator]=bloch_evolve(bloch_matrix,[linear_encoding_delay,0,0],lineshape);
                    else
                        %extra matrix operation, sacrifice performance to make
                        %code easier to read for future ed
                        bloch_delay_propogator=eye(size(bloch_propogator,1));
                    end
                    
            else
                    [bloch_propogator]=bloch_evolve(bloch_matrix,waveform);
                    
                    %if there is linear encoding, or there is a delay after presat
                    %(steady state or whatever) get the propogator for no presat as
                    %well. don't laugh at bad code please
                    if(isfield(chemical_data.experiment_list(experiment_number),'linear_encoding_delay'))
                        [bloch_delay_propogator]=bloch_evolve(bloch_matrix,[chemical_data.experiment_list(experiment_number).linear_encoding_delay,0,0]);
                    else
                        %extra matrix operation, sacrifice performance to make
                        %code easier to read for future ed
                        bloch_delay_propogator=eye(size(bloch_propogator),1);
                    end
            end
                
                
            %now rip out water value.  presat 1st operation, then delay
            %operation second
                
                M_propogated=bloch_delay_propogator*((bloch_propogator)^chemical_data.experiment_list(experiment_number).number_of_pulses)*Mnaught;
                water_magnetization=M_propogated(3);  %ignore other garbage, assumption is other pools are off resonance of the water resonance during collection of the MRI fid, if you want them this is the line in which to add them in.
                
            
            
            if ( isnan(water_magnetization) | isinf(water_magnetization) )
                water_magnetization=1;
            end
            Z_signal_array(offset_iterator)=water_magnetization;
            
        end
        % Add simulated Z signal to chemical data file while adjusting for baseline correction
        chemical_data.experiment_list(experiment_number).simulated_Z_signal = Z_signal_array./max(Z_signal_array).*chemical_data.experiment_list(experiment_number).Z_scale_factor*chemical_data.Z_scale_factor;
    end    
end %end function