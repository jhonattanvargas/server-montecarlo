'use strict'

//add models require
const addon = require('../build/Release/addon')

//add functions from routes
function serial(req, res){
  let cicles = Number(req.params.cicles)
  if(isNaN(cicles)){
    res
      .status(500)
      .send({message:'ingrese un número adecuado de ciclos a iterar'})
  }
  else{
    let obj = addon.serial(cicles)
    if(isNaN(obj.pi) || isNaN(obj.time)){
      res
        .status(500)
        .send({message:'Error al aproximar Pi'})
    }
    else{
      res
      .status(200)
      .send({
        "pi" : obj.pi,
        "timeTaken" : obj.time
      })
    }
  }
}

function parallel(req, res){
  let cicles = Number(req.params.cicles)
  if(isNaN(cicles)){
    res
      .status(500)
      .send({message:'ingrese un número adecuado de ciclos a iterar'})
  }
  else{
    let obj = addon.parallel(cicles)
    if(isNaN(obj.pi) || isNaN(obj.time) || isNaN(obj.cpus)){
      res
        .status(500)
        .send({message:'Error al aproximar Pi'})
    }
    else{
      res
      .status(200)
      .send({
        "pi" : obj.pi,
        "timeTaken" : obj.time,
        "cpus" : obj.cpus,
        "cicles" : cicles
      })
    }
  }
}

module.exports = {
  serial,
  parallel
}
