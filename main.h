/* 
 * File:   main.h
 * Author: Florent Poinsaut <florent.poinsaut@echoes-tech.com>
 *
 * Created on 13 février 2012, 16:31
 */

#ifndef MAIN_H
#define	MAIN_H

// Probe Name
#define NAME "ECHOES Alert - Sonde"
// Probe Version
#define VERSION "0.1.0"

// L'adresse IP ou idéalement le FQDN du moteur
#define SRV_FQDN "192.168.1.48"
//#define SRV_FQDN "alert.echoes-tech.com"
// Le port d'écoute du moteur
#define PORT 7171
// Le protocole d'emisssion (O : TCP, 1 : UDP)
#define PROTO 0

#endif	/* MAIN_H */

