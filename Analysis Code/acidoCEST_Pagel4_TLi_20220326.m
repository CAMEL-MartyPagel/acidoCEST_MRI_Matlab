% acidoCEST_Pagel4.m
%
% acidoCEST_Pagel4.m analyzes CEST MR images that were acquired with an
% acidoCEST MRI protocol using a Bruker MRI instrument.
% This code was written and tested by Marty Pagel and colleagues, 
% especially Ed Randtke, Vipul Sheth, Liu Qi Chen, Kyle Jones, Paul 
% Akhenblit, Leila Lindeman, Josh Goldenbeg, and Rachel High
% at the University of Arizona in 2011-2017, and at the MD Anderson 
% Cancer Center in 2018. 
%
% This Matlab code is provided at no charge 
% to the CEST MRI research community.  Although we are collegial 
% reseachers, we cannot guarantee that we can provide assistance 
% with this code. Contact Marty Pagel, mdpagel@mdanderson.org 
% or mpagel@gmail.com for details.

% Note: The acidoCEST_Pagel4.mat file must be in your
% directory defined by userpath (type 'userpath' in the 
% Command Window), or another durectory in the path used by Matlab
% (type 'path' in the Command Window). 

% TLi made edits in March 2022 so that
% (1) the Bloch fitting produces more accurate fitting result (by optimizing base catmeanalyzing parameters)
% (2) further clean up the code to remove unnecessary portion

%% Clear Workspace and define pathnames

clear all; clc; close all;
restoredefaultpath;
Starting_Directory=pwd;

%% Load and process the images
cd(Starting_Directory);
disp("Select the folder that holds all image data ");
    DIR=uigetdir; 
    DIR_finalresults=strcat(DIR,'\');
    
%% User Inputs:
prompt = {"Image Directory", "Anatomical Scan", 'Anatomical Slice for CEST', "Pre-Injection Scan", "Post-Injection Scan"};
dlgtitle = 'Mater Parameter File';
dims = [1 100; 1 20; 1 26; 1 20; 1 20];
definput = {DIR, '3', '1', '9', '10'};
user_inputs = inputdlg(prompt,dlgtitle,dims,definput);

%% If user edited image data location, correct it
DIR=user_inputs{1}; 
DIR_finalresults=strcat(DIR,'\');

%% Hard coding for now -fix future Kyle
number_of_mice=1;

for current_mouse_number=1:number_of_mice

    % Obtain information about the anatomical image 
    % that is used for selecting ROIs
    DIR_anatomical = strcat(DIR, '\', user_inputs{2}, '\');
    [anatomical_image,anatomical_offsets]=load_anatomical_image(DIR_anatomical, str2double(user_inputs{3}));
    
    % Load the pre-injection scan
    DIR_preinjection = strcat(DIR,'\', user_inputs{4}, '\');
    cd(Starting_Directory);
    [images_preinjection,images_BeforeAvg,SatFrqList,NR,MHz_value,SatFrqList_ppm,CEST_offsets,CEST_rows,CEST_columns,SatPower,SatTime,SpecAvg]=load_CEST_images_southcampus(DIR_preinjection);

    
    % Load the post-injection scan
    DIR_postinjection = strcat(DIR,'\', user_inputs{5}, '\');
    [images_postinjection,images_BeforeAvg,SatFrqList,NR,MHz_value,SatFrqList_ppm,CEST_offsets,CEST_rows,CEST_columns,SatPower,SatTime,SpecAvg]=load_CEST_images_southcampus(DIR_postinjection);
    
    SatTime=SatTime./1000;
    num_dummy=max(find(SatFrqList_ppm<-20));
    ref_image=images_postinjection(:,:,num_dummy);
    SatFrqList(1:num_dummy)=[];
    SatFrqList_ppm(1:num_dummy)=[];
    images_preinjection(:,:,1:num_dummy)=[];
    images_postinjection(:,:,1:num_dummy)=[];

    % we apply a Gaussian spatial smoothing to improve SNR.
    % An older version of this code used fspecial('gaussian',[5 5],2)
    % However, this fspecial gaussian function is no longer recommended
    % and instead imgausfilt should be used
    ref_image_gauss=imgaussfilt(ref_image,2);
    gauss_image=imgaussfilt(images_preinjection,2);
    preinjection_images(:,:,:,current_mouse_number)=gauss_image;
    gauss_image=imgaussfilt(images_postinjection,2);
    postinjection_images(:,:,:,current_mouse_number)=gauss_image;
end

%% Select Regions Of Interest and Calculate the SNR threshold

for current_mouse_number=1:number_of_mice
    
    % Show the anatomical image, and draw ROIs of the tumor and noise.
    figure()
    imagesc(anatomical_image(:,:));
    
    title(strcat('Choose tumor ROI ',num2str(current_mouse_number))); 
    finished = 'NO';
    i = 1;
    while strcmpi(finished,'NO')
      h(i) = imfreehand;
      finished = questdlg('Finished?', ...
          'confirmation', ...
          'YES', 'UNDO', 'NO');
      if strcmpi(finished, 'UNDO')
          delete(h(i))
          finished = 'NO';
      else
          tmp_mask = createMask(h);
          i = i + 1;
      end
    end
    mask_tumor=tmp_mask;


    
    title(strcat('Choose noise ROI ',num2str(current_mouse_number))); 
    finished = 'NO';
    i = 1;
    while strcmpi(finished,'NO')
      h(i) = imfreehand;
      finished = questdlg('Finished?', ...
          'confirmation', ...
          'YES', 'UNDO', 'NO');
      if strcmpi(finished, 'UNDO')
          delete(h(i))
          finished = 'NO';
      else
          tmp_mask = createMask(h);
          i = i + 1;
      end
    end
    mask_noise=tmp_mask;

    resize_factor=size(anatomical_image,1)./size(preinjection_images,1);
    if resize_factor~=1;
        mask_tumor_resize=zeros(size(preinjection_images,1),size(preinjection_images,2));
        mask_noise_resize=zeros(size(preinjection_images,1),size(preinjection_images,2));
        for aaa=1:size(mask_tumor,1)
            for bbb=1:size(mask_tumor,2)
                if mask_tumor(aaa,bbb)==1
                    mask_tumor_resize(round(aaa./resize_factor),round(bbb./resize_factor))=1;
                end
                if mask_noise(aaa,bbb)==1
                    mask_noise_resize(round(aaa./resize_factor),round(bbb./resize_factor))=1;
                end
            end
        end
    end

    mask_tumor=mask_tumor_resize;
    mask_noise=mask_noise_resize;

    image=squeeze(postinjection_images(:,:,:,current_mouse_number));
    noise_vector=[];
    for aaa=1:size(image,1)
        for bbb=1:size(image,2)
            if mask_noise(aaa,bbb)==1
                noise_vector(end+1)=ref_image_gauss(aaa,bbb);
            end
        end
    end
    std_noise=std(noise_vector);
    
    signal_vector=[];
    for aaa=1:size(image,1)
        for bbb=1:size(image,2)
            if mask_tumor(aaa,bbb)==1
                signal_vector(end+1)=ref_image_gauss(aaa,bbb);
            end
        end
    end
    mean_signal=mean(signal_vector);
    
    % I need to verify this threshold calculation - Marty Pagel.
    threshold=((std_noise)*2*sqrt(2))./mean_signal;
end

%% User needs to load in experimental information (experimental T1, T2, B1 and B0) if implementation of experimental data into fitting is desired.
B0map=[];
ROIavg_B1=[];
T1map=[];
T2map=[];

%% User defined uSec delay between end of saturation pulse and end of FISP imaging
uSec_delay = 60/10^6; % 60 uSec = 60/10^6 sec
%% Bloch fitting portion
disp('select data saving directory');
Save_Dir=uigetdir;%Change this to the desired name of saving folder
pH_results=zeros(size(preinjection_images,1),size(preinjection_images,2));
kex_results_Hz=zeros(size(preinjection_images,1),size(preinjection_images,2),2);
indices_one=find(SatFrqList_ppm>3.2);
indices_two=find(SatFrqList_ppm<6.6);
indices=intersect(indices_one,indices_two);
indices_length=length(indices);
contrast_isovue_pixel=zeros(size(preinjection_images,1),size(preinjection_images,2),indices_length);
ddd=1;
cd(Starting_Directory)
SatFrqList_Holder=SatFrqList;
SatFrqList_Holder=SatFrqList_Holder.*2.*pi;
for aaa=1:size(preinjection_images,1)
    for bbb=1:size(preinjection_images,2)
        if mask_tumor(aaa,bbb)==1
            ZSignal_preinjection = squeeze(preinjection_images(aaa, bbb, :));
            ZSignal_postinjection = squeeze(postinjection_images(aaa, bbb, :));
            %% Normalize Z spectra by the setting the maximum to 1
            ZSignal_preinjection=ZSignal_preinjection./max(ZSignal_preinjection);
            ZSignal_postinjection=ZSignal_postinjection./max(ZSignal_postinjection);
            %% Load Chemical Data file
            load('acidoCEST_Pagel4_ChemicalData_20220104_FinalVer.mat');%change this to the appropriate file name for chemical_data
            %% Feed in experimental Z signal and Saturation Frequency List into chemical data file
            chemical_data.experiment_list(1).Z_signal = ZSignal_preinjection;
            chemical_data.experiment_list(2).Z_signal = ZSignal_postinjection;
            chemical_data.experiment_list(1).Hz_offset = SatFrqList_Holder;
            chemical_data.experiment_list(2).Hz_offset = SatFrqList_Holder;

            %% Update the waveform list in the chemical_data file
            chemical_data.waveform_list(1,1) = SatTime; % Saturation Time
            chemical_data.waveform_list(1,2) = 2*pi*SatPower*42.58; % Saturation Power in Nutation Rate
            chemical_data.waveform_list(2,1) = uSec_delay; % uSec delay defined by user
            %% Make sure to update chemical_data file with parameter map info
            % Add R1 and R2 values (for water pool) to chemical_data file if you have T1 and T2 maps
            if (chemical_data.spinsystem_list(1).fit_R1 == false)
                T1 = (T1map(phantom_num))./1000;
                R1 = 1/T1;
                chemical_data.spinsystem_list(1).R1 = R1;
            end
            if (chemical_data.spinsystem_list(1).fit_R2 == false)
                T2 = (T2map(phantom_num))./1000;
                R2 = 1/T2;
                chemical_data.spinsystem_list(1).R2 = R2;
            end

            % Add B1 correction to chemical data file if you have a B1 Map
            if (chemical_data.fit_B1_correction == false)
                B1_correction = ROIavg_B1(phantom_num)/100;
                chemical_data.B1_correction = B1_correction;
            end

            % Add B0 shift information into chemical_data file if you have a B0 map. If not, estimate B0 shift with the offset of the water signal.
            if (chemical_data.B0_shift==0)
                B0_shift = B0map(phantom_num)*MHz_value*2*pi;
                chemical_data.B0_shift = B0_shift;
            else
                water_minimum_signal_index_number=find(ZSignal_preinjection==min(ZSignal_preinjection));
                water_minimum_signal_index_number=water_minimum_signal_index_number(1);
                chemical_data.B0_shift = SatFrqList(water_minimum_signal_index_number);
            end

            %% Create initial guesses, lowerbound guesses, and upperbound guesses for parameter that will be fit
            [parameters, lower_bound, upper_bound, typical_parameters, chemical_data] = initialize_parameters(chemical_data);

            %% Perform Bloch Fitting
            options_lsqnonlin = optimoptions('lsqnonlin', 'MaxFunEvals', 300000, 'MaxIter', 3000, 'TypicalX', typical_parameters, 'TolX', 1e-9, 'TolFun', 1e-9);%TLi changed the MaxFunEvals, MaxIter, TolX and TolFun on 20200607
            [final_parameters, resnorm, R, exitflag, output, lambda, J] = lsqnonlin(@(parameters) lsqnonlin_contrast_renorm(chemical_data, parameters), parameters, lower_bound, upper_bound, options_lsqnonlin);

            %% Update chemical data file with final parameters
            [chemical_data_final] = bloch_spectra_model_function(final_parameters, chemical_data);
            fitted_pH_val = chemical_data_final.experiment_list(2).pH;
            pH_results(aaa,bbb) = fitted_pH_val;
            for num = 1:2
                kex_results_Hz(aaa,bbb,num) = chemical_data_final.experiment_list(2).simulated_spinsystem_list(num+3).exchange_rate;
            end
            %% Plot fitting results
            % Fitted Difference Spectrum
            figure(1)
            raw_difference_spectrum = (chemical_data_final.experiment_list(1).Z_signal-chemical_data_final.experiment_list(2).Z_signal)';
            plot(chemical_data_final.experiment_list(2).Hz_offset, raw_difference_spectrum, 'o')
            hold on;
            simulated_difference_spectrum = chemical_data_final.experiment_list(1).simulated_Z_signal-chemical_data_final.experiment_list(2).simulated_Z_signal;
            contrast_isovue_pixel(aaa,bbb,:)=simulated_difference_spectrum(indices);
            plot(chemical_data_final.experiment_list(2).Hz_offset, simulated_difference_spectrum)
            x_coord = min(chemical_data_final.experiment_list(2).Hz_offset);
            y_coord = min(simulated_difference_spectrum)+ (max(simulated_difference_spectrum) - min(simulated_difference_spectrum))/2;
            text(x_coord, y_coord, append("Fitted pH: ", num2str(fitted_pH_val)));
            text(x_coord, y_coord+.025, append("Pixel ", num2str(ddd)));
            title("Fitted Difference Spectrum")
            cd(Save_Dir);
            saveas(gcf,strcat('FittingSpectrum_pixel',num2str(ddd),'.fig'));
            saveas(gcf,strcat('FittingSpectrum_pixel',num2str(ddd),'.png'));
            cd(Starting_Directory)
            hold off;
            pause(5)
            ddd=ddd+1;
        end
    end
end

%% Remove pixels undergoing poor fitting
pH_results_BeforeThreshold=pH_results;
kex_results_Hz_BeforeThreshold=kex_results_Hz;
for aaa=1:size(preinjection_images,1)
    for bbb=1:size(preinjection_images,2)
        if pH_results(aaa,bbb)<6.2
            pH_results(aaa,bbb)=0;
            kex_results_Hz(aaa,bbb,:)=0;
        elseif pH_results(aaa,bbb)>7.6
            pH_results(aaa,bbb)=0;
            kex_results_Hz(aaa,bbb,:)=0;
        elseif max(contrast_isovue_pixel(aaa,bbb,:))<threshold
            pH_results(aaa,bbb)=0;
            kex_results_Hz(aaa,bbb,:)=0;
        end
    end
end

cd(Save_Dir);
save pH_results_BeforeThreshold pH_results_BeforeThreshold
save kex_results_Hz_BeforeThreshold kex_results_Hz_BeforeThreshold
save pH_results pH_results;
save kex_results_Hz kex_results_Hz;