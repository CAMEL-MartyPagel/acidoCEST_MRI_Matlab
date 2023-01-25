function[bloch_propogator]=bloch_evolve(bloch_matrix,waveform,varargin)
    if(~isempty(varargin))
        lineshape_array=varargin{1};
        number_of_lineshapes=size(lineshape_array,2); %how many lineshapes
    else
        number_of_lineshapes=0;
    end
    
    number_of_spins=(size(bloch_matrix,2)-1-number_of_lineshapes)/3; %subtract dummy and lineshapes
    
    %identity matrix for no effect, we will pile all propogators onto this
    bloch_propogator=eye(size(bloch_matrix));
    
    %outer loop for the waveform, format is time, power, offset in waveform
    
    for waveform_iterator=1:length(waveform(:,1))
        
        %zero power matrix
        power_matrix=zeros(3,3);
        
        %build power matrix
        %precession
        power_matrix(1,2)=-waveform(waveform_iterator,3);
        power_matrix(2,1)=waveform(waveform_iterator,3);
        
        %waveform in y, notice the different signs because of the right hand coordinate system
        power_matrix(1,3)=imag(waveform(waveform_iterator,2));
        power_matrix(3,1)=-imag(waveform(waveform_iterator,2));
        
        %waveform in x
        power_matrix(2,3)=-real(waveform(waveform_iterator,2));
        power_matrix(3,2)=real(waveform(waveform_iterator,2));
        
        %make full power matrix
        power_matrix=kron(eye(number_of_spins),power_matrix);
        
        %lineshapes
        absorption_matrix=zeros(number_of_lineshapes);
        for lineshape_iterator=1:number_of_lineshapes
            %make lineshape matrix, i ripped this from some of my old code and do not remember why we need the pi up front.  I think it's Hz-rad lineshape conversion, so we need this for correct units
            absorption_matrix(lineshape_iterator,lineshape_iterator)=-pi*abs(waveform(waveform_iterator,2)).^2*lineshape_array(waveform_iterator,lineshape_iterator);
        end
        
        %dump in lineshape matrix
        power_matrix=blkdiag(power_matrix,absorption_matrix,0);
        
        %put all this shit into the propogator
        bloch_propogator=expm((bloch_matrix+power_matrix).*waveform(waveform_iterator,1))*bloch_propogator;
    end
end

