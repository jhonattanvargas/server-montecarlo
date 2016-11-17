'use strict'

//add models require
let addon = require('../build/Release/addon')

//add functions from routes
function serial(req, res, next){
  //console.log('serial request');
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
        "timeTaken" : obj.time,
        "cpus" : 1,
        "cicles" : cicles
      })
    }
  }
  next()
}

function parallel(req, res, next){
  //console.log('parallel request');
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
  next()
}

module.exports = {
  serial,
  parallel
}
