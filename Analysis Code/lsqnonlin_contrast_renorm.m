function[Z_collector] = lsqnonlin_contrast_renorm(chemical_data, parameters)

    % Create Simulated Z signal using parameters. Note that parameters will update until best fit is achieved
    [chemical_data] = bloch_spectra_model_function(parameters, chemical_data);
    
    % Create difference spectrum
    raw_difference_spectrum = (chemical_data.experiment_list(1).Z_signal-chemical_data.experiment_list(2).Z_signal)';
    simulated_difference_spectrum = chemical_data.experiment_list(1).simulated_Z_signal-chemical_data.experiment_list(2).simulated_Z_signal;
    
    % Take difference of simulated and raw difference spectrum to determine how close simulation fits reality
    Z_collector=(simulated_difference_spectrum-raw_difference_spectrum);
    
    % What is the purpose of this manipulation of Z signal? Not sure why this is needed but it is needed. I've tried fitting without weight and the results suck
    Z_collector=horzcat(Z_collector, (chemical_data.experiment_list(1).simulated_Z_signal(end) - chemical_data.experiment_list(1).Z_signal)');
    weights = ones(size(Z_collector));
    weights(length(Z_collector/2+1:end)) = 0.05;
    Z_collector=Z_collector.*weights;
    
end
