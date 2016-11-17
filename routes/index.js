'use strict'

const express = require('express')
const api = express.Router()
//add require controller
const calculatorCtrl = require('../controllers/calculator')

//add routes from api
api.get('/serial/:cicles',calculatorCtrl.serial)
api.get('/parallel/:cicles',calculatorCtrl.parallel)

module.exports = api
