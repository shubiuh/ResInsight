/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2016-     Statoil ASA
//
//  ResInsight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ResInsight is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
//
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Implements case realization parameters reservoir-data functionality.

#include "RigCaseRealizationParameters.h"

#include <QString>
#include <QStringList>

#include <functional>
#include <limits>

//--------------------------------------------------------------------------------------------------
/// Creates a Value instance.
//--------------------------------------------------------------------------------------------------
RigCaseRealizationParameters::Value::Value()
    : m_valueType( TYPE_NONE )
    , m_numericValue( std::numeric_limits<double>::infinity() )
{
}

//--------------------------------------------------------------------------------------------------
/// Creates a Value instance.
//--------------------------------------------------------------------------------------------------
RigCaseRealizationParameters::Value::Value( double value )
    : Value()
{
    setValue( value );
}

//--------------------------------------------------------------------------------------------------
/// Creates a Value instance.
//--------------------------------------------------------------------------------------------------
RigCaseRealizationParameters::Value::Value( const QString& value )
    : Value()
{
    setValue( value );
}

//--------------------------------------------------------------------------------------------------
/// Sets value.
//--------------------------------------------------------------------------------------------------
void RigCaseRealizationParameters::Value::setValue( double value )
{
    m_valueType    = TYPE_NUMERIC;
    m_numericValue = value;
}

//--------------------------------------------------------------------------------------------------
/// Sets value.
//--------------------------------------------------------------------------------------------------
void RigCaseRealizationParameters::Value::setValue( const QString& value )
{
    m_valueType = TYPE_TEXT;
    m_textValue = value;
}

//--------------------------------------------------------------------------------------------------
/// Returns the numeric value.
//--------------------------------------------------------------------------------------------------
double RigCaseRealizationParameters::Value::numericValue() const
{
    return m_numericValue;
}

//--------------------------------------------------------------------------------------------------
/// Returns the text value.
//--------------------------------------------------------------------------------------------------
const QString& RigCaseRealizationParameters::Value::textValue() const
{
    return m_textValue;
}

//--------------------------------------------------------------------------------------------------
/// Adds parameter.
//--------------------------------------------------------------------------------------------------
void RigCaseRealizationParameters::addParameter( const QString& name, double value )
{
    m_parameters[name].setValue( value );
}

//--------------------------------------------------------------------------------------------------
/// Adds parameter.
//--------------------------------------------------------------------------------------------------
void RigCaseRealizationParameters::addParameter( const QString& name, const QString& value )
{
    m_parameters[name].setValue( value );
}

//--------------------------------------------------------------------------------------------------
/// Returns the parameter value.
//--------------------------------------------------------------------------------------------------
RigCaseRealizationParameters::Value RigCaseRealizationParameters::parameterValue( const QString& name )
{
    if ( m_parameters.count( name ) == 0 ) return Value();
    return m_parameters[name];
}

//--------------------------------------------------------------------------------------------------
/// Returns the parameters.
//--------------------------------------------------------------------------------------------------
std::map<QString, RigCaseRealizationParameters::Value> RigCaseRealizationParameters::parameters() const
{
    return m_parameters;
}

//--------------------------------------------------------------------------------------------------
/// Returns the parameter names.
//--------------------------------------------------------------------------------------------------
std::set<QString> RigCaseRealizationParameters::parameterNames() const
{
    std::set<QString> names;
    for ( auto& par : parameters() )
        names.insert( par.first );
    return names;
}

//--------------------------------------------------------------------------------------------------
/// Returns the parameter hash.
//--------------------------------------------------------------------------------------------------
size_t RigCaseRealizationParameters::parameterHash( const QString& name ) const
{
    auto itr = m_parameters.find( name );
    if ( itr == m_parameters.end() || !itr->second.isValid() ) return 0;

    std::hash<std::string> stringHasher;
    std::hash<double>      doubleHasher;
    size_t                 nameHash;
    size_t                 valueHash = 0;

    nameHash = stringHasher( name.toStdString() );

    auto value = itr->second;
    if ( value.isNumeric() )
    {
        valueHash = doubleHasher( value.numericValue() );
    }
    else if ( value.isText() )
    {
        valueHash = stringHasher( value.textValue().toStdString() );
    }

    QString s = QString::number( nameHash ) + QString::number( valueHash );

    return stringHasher( s.toStdString() );
}

//--------------------------------------------------------------------------------------------------
/// Returns the parameters hash.
//--------------------------------------------------------------------------------------------------
size_t RigCaseRealizationParameters::parametersHash()
{
    if ( m_parametersHash == 0 ) calculateParametersHash();
    return m_parametersHash;
}

//--------------------------------------------------------------------------------------------------
/// Returns the realization number.
//--------------------------------------------------------------------------------------------------
int RigCaseRealizationParameters::realizationNumber() const
{
    return m_realizationNumber;
}

//--------------------------------------------------------------------------------------------------
/// Sets realization number.
//--------------------------------------------------------------------------------------------------
void RigCaseRealizationParameters::setRealizationNumber( int realization )
{
    m_realizationNumber = realization;
}

//--------------------------------------------------------------------------------------------------
/// Clears parameters hash.
//--------------------------------------------------------------------------------------------------
void RigCaseRealizationParameters::clearParametersHash()
{
    m_parametersHash = 0;
}

//--------------------------------------------------------------------------------------------------
/// Calculates parameters hash.
//--------------------------------------------------------------------------------------------------
void RigCaseRealizationParameters::calculateParametersHash( const std::set<QString>& paramNames /*= std::set<QString>()*/ )
{
    QStringList hashes;

    if ( paramNames.empty() )
    {
        for ( auto param : m_parameters )
        {
            hashes.push_back( QString::number( parameterHash( param.first ) ) );
        }
    }
    else
    {
        for ( const auto& paramName : paramNames )
        {
            if ( m_parameters.find( paramName ) == m_parameters.end() ) return;
            hashes.push_back( QString::number( parameterHash( paramName ) ) );
        }
    }

    std::hash<std::string> stringHasher;
    m_parametersHash = stringHasher( hashes.join( "" ).toStdString() );
}
